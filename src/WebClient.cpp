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

#include <thread>
#include <Maraton.h>
#include "WebClient.h"

NS_MARATON_BEGIN

WebClient::WebClient( )
{
    this->Header( "Connection" , "close" );
    this->Header( "Author" , "YHGenomics/Maraton" );
    this->Header( "VHTTP" , "Alpha/0.0.1" );
    this->Header( "Accept" , "*/*" );

    uv_loop_init( &this->uv_loop_ );
}

void WebClient::Get( std::string url ,
                     CallbackResponseType callback )
{
    uptr<HTTPRequest> req               = make_uptr( HTTPRequest , 
                                                     url , 
                                                     "GET");
    uptr<WebClientRequestToken> token   = make_uptr( WebClientRequestToken );
    token->req_                         = move_ptr( req );
    token->callback_                    = callback;
    token->uv_loop                      = Maraton::Instance()->Event()->Event();

    this->FillHeader( token->req_.get( ) );
    
    this->QueryDns( move_ptr( token ) );
}

void WebClient::GetX( std::string url , CallbackStreamType callback )
{
    uptr<HTTPRequest> req               = make_uptr( HTTPRequest , 
                                                     url , 
                                                     "GET");
    uptr<WebClientRequestToken> token   = make_uptr( WebClientRequestToken );
    token->req_                         = move_ptr( req );
    token->rep_                         = make_uptr( HTTPResponse ); 
    //token->callback_                    = callback;
    token->uv_loop                      = Maraton::Instance()->Event()->Event();
   
    token->rep_->ReadCallback( [callback] ( HTTPResponse* rep ,
                               uptr<Buffer>  buf )
    {
        callback( rep , move_ptr( buf ) );
    });

    this->FillHeader( token->req_.get( ) );

    this->QueryDns( move_ptr( token ) );
}

void WebClient::Post( std::string url , 
                      std::string data , 
                      CallbackResponseType callback )
{
    uptr<HTTPRequest> req               = make_uptr( HTTPRequest ,
                                                     url , 
                                                     "POST");
    uptr<WebClientRequestToken> token   = make_uptr( WebClientRequestToken );
    token->req_                         = move_ptr( req );
    token->callback_                    = callback;
    token->uv_loop                      = Maraton::Instance()->Event()->Event();
    
    this->FillHeader     ( token->req_.get( ) );
    token->req_->Content ( make_uptr( Buffer , data ) );
    this->QueryDns       ( move_ptr( token ) );
}

void WebClient::PostX( std::string url , std::string data , CallbackStreamType callback )
{
    uptr<HTTPRequest> req               = make_uptr( HTTPRequest , 
                                                     url , 
                                                     "POST");
    uptr<WebClientRequestToken> token   = make_uptr( WebClientRequestToken );
    token->req_                         = move_ptr( req );
    token->rep_                         = make_uptr( HTTPResponse ); 
    //token->callback_                    = callback;
    token->uv_loop                      = Maraton::Instance()->Event()->Event();
   
    token->rep_->ReadCallback( [callback] ( HTTPResponse* rep ,
                               uptr<Buffer>  buf )
    {
        callback( rep , move_ptr( buf ) );
    });

    this->FillHeader( token->req_.get( ) );

    this->QueryDns( move_ptr( token ) );
}

void WebClient::PostFile( std::string url , 
                          FILE * pfile ,
                          CallbackResponseType callback )
{
    uptr<HTTPRequest> req               = make_uptr( HTTPRequest ,
                                                     url ,
                                                     "POST");
    uptr<WebClientRequestToken> token   = make_uptr( WebClientRequestToken );
    token->req_                         = move_ptr( req );
    token->callback_                    = callback;
    token->uv_loop                      = Maraton::Instance()->Event()->Event();
    
    req->Data( pfile );
    this->FillHeader( token->req_.get( ) );

    fseek( pfile , 0 , SEEK_END );
    size_t file_size = ftell( pfile );
    fseek( pfile , 0 , SEEK_SET );
    
    req->ContentLength( file_size );

    req->WriteCallback( [ ] ( HTTPRequest * request )
    { 
        FILE* pfile = scast<FILE*>( request->Data( ) );
        char file_buffer[1024 * 1024] = { 0 };
        size_t reads = fread( file_buffer , 
                              1024 * 1024 , 
                              1 , 
                              pfile );
        return make_uptr( Buffer , file_buffer , reads );
    } );

    
    this->QueryDns( move_ptr( token ) );
}

void WebClient::DownloadFile( std::string url ,
                         FILE* pfile ,
                         CallbackResponseType callback)
{ 
    uptr<HTTPRequest> req               = make_uptr( HTTPRequest , 
                                                     url , 
                                                     "GET");
    uptr<WebClientRequestToken> token   = make_uptr( WebClientRequestToken );
    token->req_                         = move_ptr( req );
    token->rep_                         = make_uptr( HTTPResponse ); 
    token->callback_                    = callback;
    token->uv_loop                      = Maraton::Instance()->Event()->Event();
   
    token->rep_->ReadCallback( [pfile] ( HTTPResponse* rep ,
                               uptr<Buffer>  buf )
    {
        fwrite( buf->Data( ) , 1 ,  buf->Size( ) , pfile );
    });

    this->FillHeader( token->req_.get( ) );

    this->QueryDns( move_ptr( token ) );
}

uptr<HTTPResponse> WebClient::GetSync( std::string url )
{   
    bool isWaiting                      = true;
    uptr<HTTPResponse> ret              = nullptr;
    uptr<HTTPRequest> req               = make_uptr( HTTPRequest , 
                                                     url , 
                                                     "GET");
    uptr<WebClientRequestToken> token   = make_uptr( WebClientRequestToken );
    token->req_                         = move_ptr( req );
    token->callback_                    = [ &isWaiting , &ret ] ( uptr<HTTPResponse> rep)
    { 
        isWaiting = false;
        ret = move_ptr( rep );
    };
    token->uv_loop                      = &this->uv_loop_;

    this->FillHeader( token->req_.get( ) );
    
    this->QueryDns( move_ptr( token ) );

    while ( isWaiting )
    {
        uv_run( &this->uv_loop_ , UV_RUN_ONCE );
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }

    return move_ptr( ret );
}

uptr<HTTPResponse> WebClient::PostSync( std::string url , std::string data )
{
    bool isWaiting                      = true;
    uptr<HTTPResponse> ret              = nullptr;
    
    uptr<HTTPRequest> req               = make_uptr( HTTPRequest ,
                                                     url , 
                                                     "POST");
    uptr<WebClientRequestToken> token   = make_uptr( WebClientRequestToken );
    token->req_                         = move_ptr( req );
    token->callback_                    = [ &isWaiting , &ret ] ( uptr<HTTPResponse> rep)
    { 
        isWaiting = false;
        ret = move_ptr( rep );
    };
    token->uv_loop                      = &this->uv_loop_;
    
    this->FillHeader     ( token->req_.get( ) );
    token->req_->Content ( make_uptr( Buffer , data ) );
    this->QueryDns       ( move_ptr( token ) );

    while ( isWaiting )
    {
        uv_run( &this->uv_loop_ , UV_RUN_ONCE );
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }

    return move_ptr( ret );
}

void WebClient::Header( std::string key , std::string value )
{
    this->header_[key] = value;
}

void WebClient::QueryDns( uptr<WebClientRequestToken> t )
{
    auto token = t.release( );

    if ( token->rep_ == nullptr )
        token->rep_                  = make_uptr( HTTPResponse );

    token->uv_tcp.data              = token;
    token->uv_connect.data          = token;
    token->addrinfo_.ai_family      = PF_INET;
    token->addrinfo_.ai_socktype    = SOCK_STREAM;
    token->addrinfo_.ai_protocol    = IPPROTO_TCP;
    token->addrinfo_.ai_flags       = 0;
    token->uv_getaddrinfo.data      = token;  

    int r = uv_getaddrinfo( token->uv_loop ,
                            &token->uv_getaddrinfo ,
                            WebClient::uv_process_resolved ,
                            token->req_->Domain( ).c_str( ) ,
                            NULL ,
                            &token->addrinfo_ );

    if ( r < 0 )
    {
        Logger::Error( "WebClient %" , uv_strerror(r) );
    }
}

void WebClient::FillHeader( HTTPRequest * req )
{
    for( auto kv : this->header_ )
    {
        req->Header( kv.first , kv.second );
    }
}

void WebClient::uv_send_request( WebClientRequestToken* token )
{
    uptr<Buffer> header_buf     = token->req_->BuildHeader( );
    uptr<Buffer> body_buf       = token->req_->BuildBody( );

    write_token_t* write_token  = new write_token_t;
    write_token->writer         = new uv_write_t();
    write_token->buffer         = new uv_buf_t();
    write_token->buffer->base   = new char[header_buf->Size() + 1] { 0 };
    write_token->buffer->len    = (unsigned long)header_buf->Size();
    write_token->writer->data   = write_token;
    write_token->session        = token;

    memcpy( write_token->buffer->base, 
            header_buf->Data(), 
            header_buf->Size() );
    
    auto r  = uv_write( write_token->writer, 
                        (uv_stream_t*) &token->uv_tcp, 
                        write_token->buffer, 
                        1,  
                        WebClient::uv_write_callback );
    //LOG_DEBUG_UV( r ); 

    if ( body_buf != nullptr &&
         body_buf->Size() > 0)
    {
        write_token                 = new write_token_t;
        write_token->writer         = new uv_write_t( );
        write_token->buffer         = new uv_buf_t( );
        write_token->buffer->base   = new char[body_buf->Size( ) + 1] { 0 };
        write_token->buffer->len    = (unsigned long)body_buf->Size( );
        write_token->writer->data   = write_token;
        write_token->session        = token;

        memcpy( write_token->buffer->base ,
                body_buf->Data( ) ,
                body_buf->Size( ) );

        r  = uv_write( write_token->writer ,
                       ( uv_stream_t* ) &token->uv_tcp ,
                       write_token->buffer ,
                       1 ,
                       WebClient::uv_write_callback );
        
        if ( r < 0 )
        {
            Logger::Error( "WebClient %" , uv_strerror(r) ); 
        }
    }
}

void WebClient::uv_process_resolved( uv_getaddrinfo_t * req , 
                                     int status , 
                                     addrinfo * res )
{
    int result = 0;
    WebClientRequestToken *
        token = scast<WebClientRequestToken*>( req->data ); 

    if ( token == nullptr )
    {
        //LOG_DEBUG( "operator is nullptr" );
        return;
    }

    if ( status < 0 )
    {
        Logger::Error( "WebClient %" , uv_strerror(status) );
        token->InvokeCallback( nullptr );
        SAFE_DELETE( token );
        delete res;
        return;
    }

    char ip[17] = { 0 };

    uv_ip4_name( ( struct sockaddr_in* ) res->ai_addr ,
                 ip ,
                 16 );
    delete res;
    uv_ip4_addr( ip ,
                 token->req_->port_ ,
                 &token->addr_in_ );

    token->ip_ = ip;

    token->req_->Header( "Host" , token->req_->domain_ );

    uv_tcp_init( token->uv_loop , &token->uv_tcp );

    result = uv_tcp_connect( &token->uv_connect ,
                             &token->uv_tcp ,
                             ( const struct sockaddr* ) &token->addr_in_ ,
                             WebClient::uv_connected_callback );
    if ( result < 0 )
    {
        Logger::Error( "WebClient %" , uv_strerror(result) );
        token->InvokeCallback( nullptr );
        SAFE_DELETE( token );
    }
}

void WebClient::uv_connected_callback( uv_connect_t * req , int status )
{
    WebClientRequestToken * 
        token = scast<WebClientRequestToken*>( req->data );

    if ( status < 0 )
    {
        Logger::Error( "WebClient %" , uv_strerror(status) );
        uv_close( ( uv_handle_t* ) &token->uv_tcp ,
                  WebClient::uv_close_callback );
        return;
    }

    if ( token == nullptr )
    {
        //LOG_DEBUG( "WebClientRequestToken is nullptr!" );
        return;
    }

    auto result = uv_read_start( ( uv_stream_t* ) &token->uv_tcp ,
                                 WebClient::uv_alloc_callback ,
                                 WebClient::uv_read_callback );

    WebClient::uv_send_request( token );

    if ( result < 0 )
    {
        Logger::Error( "WebClient %" , uv_strerror(result) );
        uv_close( ( uv_handle_t* ) &token->uv_tcp ,
                  WebClient::uv_close_callback );
    }
}

void WebClient::uv_alloc_callback( uv_handle_t * handle , 
                                   size_t suggested_size , 
                                   uv_buf_t * buf )
{
    buf->base = new char[suggested_size];
    buf->len  = (unsigned long)suggested_size;
}

void WebClient::uv_read_callback( uv_stream_t * stream ,
                                  ssize_t nread , 
                                  const uv_buf_t * buf )
{
    WebClientRequestToken 
        * token = scast<WebClientRequestToken*>( stream->data );

    if ( token == nullptr )
    {
        delete buf->base ;
        return;
    }

    if ( nread < 0 )
    {
        delete buf->base ;
        uv_close( ( uv_handle_t* ) &token->uv_tcp ,
                  WebClient::uv_close_callback );
        return;
    }

    if( nread > 0 )
        token->rep_->Parse( make_uptr( Buffer , buf->base , nread ) );

    delete buf->base ;
}

void WebClient::uv_write_callback( uv_write_t * req , int status )
{
    write_token_t* write_token = scast<write_token_t*>( req->data );
    
    if( write_token == nullptr )
    {
        return;
    }

    uv_buf_t* buffer           = write_token->buffer;

    if ( status < 0 )
    {
        //LOG_DEBUG_UV( status );
        SAFE_DELETE( buffer->base );
        SAFE_DELETE( buffer );
        SAFE_DELETE( write_token );
        SAFE_DELETE( req );
        return;
    }

    SAFE_DELETE( buffer->base );
    SAFE_DELETE( buffer );
    SAFE_DELETE( write_token );
    SAFE_DELETE( req );
}

void WebClient::uv_close_callback( uv_handle_t * handle )
{
    WebClientRequestToken * 
        token = scast<WebClientRequestToken*>( handle->data );

    token->InvokeCallback( move_ptr( token->rep_ ) );

    SAFE_DELETE( token );
}

NS_MARATON_END
