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

#include "Session.h"
#include "Operator.h"
#include "SyncWorker.h"

#ifndef _WIN32
#include <string.h>
#endif

NS_MARATON_BEGIN

Session::Session( )
{
    is_connected_ = true;
}

Session::~Session( )
{

}

void Session::Close( )
{
    if ( !is_connected_ )
        return;

    this->is_connected_=false;
    this->opt_->CloseSession( this );
}

void Session::Send( uptr<Buffer> pBuffer )
{ 
    if ( !is_connected_ )
        return;

    auto data = move_ptr( pBuffer );

    if ( data == nullptr )
    {
        return;
    }

    write_token_t* write_token  = new write_token_t( );
    write_token->writer         = new uv_write_t( );
    write_token->buffer         = new uv_buf_t( );
    write_token->buffer->base   = new char[data->Size( )] { 0 };
    write_token->buffer->len    = (unsigned long)data->Size( );
    write_token->writer->data   = write_token;
    write_token->session        = this;

    memcpy( write_token->buffer->base , data->Data( ) , data->Size( ) );

    auto r  = uv_write( write_token->writer ,
                        ( uv_stream_t* ) &this->uv_tcp_ ,
                        write_token->buffer ,
                        1 ,
                        Session::uv_write_callback );

    //LOG_DEBUG_UV( r );
}

void Session::uv_write_callback( uv_write_t * req , int status )
{
    write_token_t* write_token = scast<write_token_t*>( req->data );
      
    if( write_token == nullptr )
    {
        //LOG_DEBUG( "write_token is nullptr!" );
        return;
    }

    uv_buf_t* buffer           = write_token->buffer;

    if ( status < 0 )
    {
        write_token->session->error_.Code( status );
        write_token->session->error_.Message( uv_strerror(status) );
         
        SAFE_DELETE( buffer->base );
        SAFE_DELETE( buffer );
        SAFE_DELETE( write_token );
        SAFE_DELETE( req );
        return;
    }

    write_token->session->OnWrite( make_uptr( Buffer , 
                                    write_token->buffer->base , 
                                    write_token->buffer->len ) );
    
    SAFE_DELETE( buffer->base );
    SAFE_DELETE( buffer );
    SAFE_DELETE( write_token );
    SAFE_DELETE( req );
} 

NS_MARATON_END
