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

#include "AsyncWorker.h" 
#include <Maraton.h>

NS_MARATON_BEGIN

AsyncWorker* AsyncWorker::Create( CallbackType acting ,
                                  CallbackType finish , 
                                  void * data )
{
    auto r = new AsyncWorker( acting, finish );
    r->Data( data );
    r->Start();
    return r;
}

AsyncWorker* AsyncWorker::Create( CallbackType callback, 
                                  void * data )
{
    auto r = new AsyncWorker( callback , nullptr );
    r->Data( data );
    r->Start();
    return r;
}

void AsyncWorker::Stop( AsyncWorker * worker )
{
    if ( worker == nullptr ) return;

    worker->Stop( );
    SAFE_DELETE( worker );
}

void AsyncWorker::Stop( )
{
    uv_cancel( (uv_req_t*) &this->worker_ );
}

AsyncWorker::AsyncWorker( CallbackType acting ,
                          CallbackType finish )
{
    this->acting_callback_ = acting;
    this->finish_callback_ = finish;
}

void AsyncWorker::Start()
{
    this->worker_.data = this;
    uv_queue_work( Maraton::Instance()->Event()->Event() , &this->worker_ ,
                   AsyncWorker::uv_process_work_callback , 
                   AsyncWorker::uv_process_after_work_callback );
}



void AsyncWorker::uv_process_work_callback( uv_work_t * req )
{
    AsyncWorker* inst = static_cast< AsyncWorker* >( req->data );

    if ( inst == nullptr )return;
    
    inst->acting_callback_( inst );
}

void AsyncWorker::uv_process_after_work_callback( uv_work_t * req , int status )
{
    AsyncWorker* inst = static_cast< AsyncWorker* >( req->data );

    if ( inst == nullptr )return;

    if ( inst->finish_callback_ != nullptr )
    {
        inst->finish_callback_( inst );
    }

    SAFE_DELETE( inst );
}

NS_MARATON_END