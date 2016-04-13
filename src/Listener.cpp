/***********************************************************************************
This file is part of Project for MaratonFramework
For the latest info, see  https://github.com/Yhgenomics/MaratonFramework.git

Copyright 2016 Yhgenomics

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
***********************************************************************************/

/***********************************************************************************
* Description   : 
* Creator       : 
* Date          : 
* Modifed       : When      | Who       | What
***********************************************************************************/

#include "Listener.h"

#ifndef _WIN32
#include <string.h>
#endif

NS_MARATON_BEGIN 
 
Listener::Listener( std::string addr , int port )
    : Operator( addr , port )
{
    this->uv_tcp_.data = this;
}

Listener::~Listener()
{

}

void Listener::DoWork()
{
    auto result = uv_tcp_init( this->uv_loop_ ,
                               &this->uv_tcp_ );
    //LOG_DEBUG_UV( result );
      
    result = uv_tcp_bind( &this->uv_tcp_ ,
                          ( const struct sockaddr* )&this->addr_in_ ,
                          0 );
    //LOG_DEBUG_UV( result );

    result = uv_listen  ( ( uv_stream_t* ) &this->uv_tcp_ ,
                          MAX_CONNECTION_SIZE ,
                          Listener::uv_new_connection_callback );
    //LOG_DEBUG_UV( result );
} 
 
void Listener::uv_new_connection_callback( uv_stream_t * server , int status )
{
    Listener* listener = scast<Listener*>( server->data );

    if ( status < 0 )
    {
        //LOG_DEBUG_UV( status );
        return;
    }

    if ( listener == nullptr )
    {
        //LOG_DEBUG( "Listener is nullptr!" );
        return;
    }

    auto session = listener->CreateSession( );

    uv_tcp_init             ( listener->uv_loop_ , 
                              &session->uv_tcp_ );

    session->uv_tcp_.data   = session;
    session->opt_           = listener;

    auto r = uv_accept( server , ( uv_stream_t* ) &session->uv_tcp_ );

    struct sockaddr  peername;
    struct sockaddr_in* peer_addr;
    int namelen;
    memset(&peername, -1, sizeof peername);
	namelen = sizeof peername;
	r = uv_tcp_getpeername(&session->uv_tcp_, &peername, &namelen);
    peer_addr = (sockaddr_in*)&peername;
    
    auto ip = inet_ntoa( peer_addr->sin_addr );
    session->ip_address_ = ip;
    session->port_ = peer_addr->sin_port;

    if ( r == 0 )
    { 
        r = uv_read_start( (uv_stream_t*)&session->uv_tcp_ , 
                           Listener::uv_alloc_callback , 
                           Listener::uv_read_callback );
        
        listener->OnSessionOpen( session );
        session->OnConnect( );
    }
    else
    {
        uv_close( ( uv_handle_t* ) &session->uv_tcp_ , 
                  Listener::uv_close_callback );
    }
}

void Listener::uv_alloc_callback( uv_handle_t * handle , 
                                  size_t suggested_size , 
                                  uv_buf_t * buf )
{
    buf->base = new char[suggested_size];
    buf->len  = suggested_size;
}

void Listener::uv_read_callback( uv_stream_t * stream , 
                                 ssize_t nread , 
                                 const uv_buf_t * buf )
{
    Session* session = scast<Session*>( stream->data );

    if ( session == nullptr )
    {
        //LOG_DEBUG( "Session is nullptr!" );
        delete buf->base;
        return;
    }

    if ( nread < 0 )
    {
        //LOG_DEBUG_UV( nread );
        session->error_.Code( nread );
        session->error_.Message( uv_strerror( ( int ) nread ) );

        uv_close( ( uv_handle_t* ) &session->uv_tcp_ ,
                  Listener::uv_close_callback );
        delete buf->base;
        return;
    }

    uptr<Buffer> pbuf = make_uptr( Buffer , buf->base , nread );
    session->OnRead( move_ptr( pbuf ) );

    delete buf->base;
}

void Listener::uv_close_callback( uv_handle_t * handle )
{
    Session* session = scast<Session*>( handle->data );
   
    if ( session == nullptr )
    {
        //LOG_DEBUG( "Session is nullptr!" );
        return;
    }

    Listener* listener = scast<Listener*>( session->opt_ );

    if ( listener == nullptr )
    {
        //LOG_DEBUG( "Listener is nullptr!" );
        return;
    }

    session->OnClose           ( );
    listener->OnSessionClose  ( session );

    //for ( size_t i = 0; i < listener->session_list_index_; i++ )
    //{
    //    if( listener->session_list_[i] == session )
    //    {
    //        SAFE_DELETE( listener->session_list_[i] );
    //        if( (listener->session_list_index_ - 1) == i )
    //        {
    //            --listener->session_list_index_;
    //        }
    //        break;
    //    }
    //} 
}

void Listener::CloseSession( Session * session )
{
    uv_close( ( uv_handle_t* ) &session->uv_tcp_ , 
              Listener::uv_close_callback );
}
 
NS_MARATON_END
