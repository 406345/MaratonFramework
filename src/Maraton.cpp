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

#include "Maraton.h"
#include "uv.h"

NS_MARATON_BEGIN

void Maraton::Regist( sptr<Operator> opt )
{
    opt->addrinfo_.ai_family       = PF_INET;
    opt->addrinfo_.ai_socktype     = SOCK_STREAM;
    opt->addrinfo_.ai_protocol     = IPPROTO_TCP;
    opt->addrinfo_.ai_flags        = 0;
    opt->uv_getaddrinfo_.data      = opt.get( );
    opt->uv_loop_                  = this->uv_loop( );

    int r = uv_getaddrinfo( opt->uv_loop_ ,
                            &opt->uv_getaddrinfo_ ,
                            Maraton::uv_process_resolved ,
                            opt->address_.c_str( ) ,
                            NULL ,
                            &opt->addrinfo_ );

    opt->index_             = elements_index_;
    elements_[opt->index_]  = sptr<Operator>( opt );
    elements_index_         = ( elements_index_ + 1 ) % MAX_CONNECTION_SIZE;
}

void Maraton::uv_process_resolved( uv_getaddrinfo_t * req , int status , addrinfo * res )
{
    int result      = 0;
    Operator * opt  = scast<Operator*>( req->data );

    if ( opt == nullptr )
    {
        //LOG_DEBUG( "operator is nullptr" );
        return;
    } 

    if( status < 0 )
    {
        //LOG_DEBUG_UV( status );
        Maraton::Instance( )->Unregist( opt );
        delete res;
        return;
    }

    char ip[17] = { 0 };

    uv_ip4_name( ( struct sockaddr_in* ) res->ai_addr , 
                 ip , 
                 16 );
    uv_ip4_addr( ip , 
                 opt->port_ , 
                 &opt->addr_in_ );
    opt->ip_ = ip;

    opt->DoWork();

    req->data = nullptr;
    delete res;
}

void Maraton::Unregist( sptr<Operator> opt )
{
    this->Unregist( opt.get( ) );
}

void Maraton::Run( )
{
    this->loop_event->Wait();
}

void Maraton::Unregist( const Operator * opt )
{
    if ( opt == nullptr )
        return;

    elements_[opt->index_] = nullptr;
}

uv_loop_t * Maraton::uv_loop( )
{
    return loop_event->Event();
}

NS_MARATON_END