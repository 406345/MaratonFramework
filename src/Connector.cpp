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

#include "Connector.h"
#include "Maraton.h"

NS_MARATON_BEGIN

Connector::Connector( std::string addr , int port )
    : Operator( addr , port )
{

}

Connector::~Connector( )
{

}

void Connector::DoWork( )
{
    this->session_          = this->CreateSession( );
    this->session_->opt_    = this;

    uv_tcp_init( this->uv_loop_ ,
                 &this->session_->uv_tcp_ );

    this->session_->uv_tcp_.data    = this->session_;
    this->uv_connect_.data          = this;

    auto result = uv_tcp_connect( &this->uv_connect_ ,
                                  &this->session_->uv_tcp_ ,
                                  ( const struct sockaddr* ) &this->addr_in_ ,
                                  Connector::uv_connected_callback );
    //LOG_DEBUG_UV( result );
}

void Connector::uv_connected_callback( uv_connect_t * req , int status )
{
    Connector* opt = scast<Connector*>( req->data );
    
    if ( opt == nullptr )
    {
        //LOG_DEBUG( "Session is nullptr!" );
        return;
    }

    if ( status < 0 )
    {
        //LOG_DEBUG_UV( status );
        opt->session_->error_.Code( status );
        opt->session_->error_.Message( uv_strerror(status) );

        uv_close( ( uv_handle_t* ) &opt->session_->uv_tcp_ ,
                  Connector::uv_close_callback );
        return;
    }

    

    opt->OnSessionOpen        ( opt->session_ );
    opt->session_->OnConnect   ( );

    auto result = uv_read_start( ( uv_stream_t* ) &opt->session_->uv_tcp_ ,
                                 Connector::uv_alloc_callback ,
                                 Connector::uv_read_callback );
    //LOG_DEBUG_UV( result );
}

void Connector::uv_alloc_callback( uv_handle_t * handle , size_t suggested_size , uv_buf_t * buf )
{
    buf->base = new char[suggested_size];
    buf->len  = suggested_size;
}

void Connector::uv_read_callback( uv_stream_t * stream , ssize_t nread , const uv_buf_t * buf )
{
    Session* session = scast<Session*>( stream->data );

    if ( session == nullptr )
    {
        //LOG_DEBUG( "Session is nullptr!" );
        return;
    }

    if ( nread < 0 )
    {
        //LOG_DEBUG_UV( nread );
        session->error_.Code( nread );
        session->error_.Message( uv_strerror(nread) );

        uv_close( ( uv_handle_t* ) &session->uv_tcp_ , Connector::uv_close_callback );
        return;
    }

    uptr<Buffer> pbuf = make_uptr( Buffer , buf->base , nread );
    session->OnRead( move_ptr( pbuf ) );

    delete buf->base;
}
void Connector::uv_close_callback( uv_handle_t * handle )
{
    Session* session = scast<Session*>( handle->data );

    if ( session == nullptr )
    {
        //LOG_DEBUG( "Session is nullptr!" );
        return;
    }

    auto opt = scast<Connector*>( session->opt_ );
     
    session->OnClose( );
    opt->OnSessionClose( session );
    Maraton::Instance( )->Unregist( opt );
} 

void Connector::CloseSession( Session * session )
{
    uv_close( ( uv_handle_t* ) &session->uv_tcp_ , Connector::uv_close_callback );
}

NS_MARATON_END
