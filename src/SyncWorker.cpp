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

#include "SyncWorker.h"

NS_MARATON_BEGIN

SyncWorker* SyncWorker::Create( const SyncworkerCallbackType  work_callback , 
                         const SyncworkerCallbackType  after_callback , 
                         void * data )
{
    SyncWorker* worker      = new SyncWorker();
    worker->cb_work_        = work_callback;
    worker->cb_after_work_  = after_callback;
    worker->loop_time_      = 1;
    worker->Data( data );
    worker->Start();

    return worker;
}

SyncWorker* SyncWorker::Create( const size_t time_span , 
                         const SyncworkerCallbackType work_callback , 
                         const SyncworkerCallbackType after_callback , 
                         void * data )
{
    SyncWorker* worker      = new SyncWorker();
    worker->cb_work_        = work_callback;
    worker->cb_after_work_  = after_callback;
    worker->loop_time_      = time_span;
    worker->Data( data );
    worker->Start();

    return worker;
}

void SyncWorker::Stop( SyncWorker * worker )
{
    if ( worker == nullptr )
    {
        return;
    }

    worker->Stop( );
}

void SyncWorker::Stop()
{
    this->finished_ = true;
}

void SyncWorker::uv_process_timer_tick_callback( uv_timer_t * handle )
{
    SyncWorker* worker = static_cast< SyncWorker* >( handle->data );

    if ( worker->finished_ )
    { 
        if ( worker->cb_after_work_ != nullptr )
        {
            worker->cb_after_work_( worker );
        }

        int result = uv_timer_stop( &worker->timer_ );

        LOG_DEBUG_UV( result );
        
        SAFE_DELETE( worker );

        return;
    }

    if( worker->loop_count_ == 0)
    {
        worker->loop_count_++;
        return;
    }

    if ( worker == nullptr )
    {
        uv_timer_stop( handle );
        return;
    }

    if ( !worker->finished_ )
    {
        worker->finished_  = worker->cb_work_( worker );
        ++worker->loop_count_;
    }

    /*if ( worker->finished_ )
    { 
        if ( worker->cb_after_work_ != nullptr )
        {
            worker->cb_after_work_( worker );
        }

        int result_ = uv_timer_stop( &worker->timer_ );

        LOG_DEBUG_UV( result_ );
        
        SAFE_DELETE( worker );
    }*/
}

SyncWorker::SyncWorker()
{
    this->timer_        = { 0 };
    this->timer_.data   = this;
    this->loop_count_   = 0;
    this->loop_time_    = 1;

    uv_timer_init( uv_default_loop() , &this->timer_ );
}

SyncWorker::~SyncWorker()
{
}

void SyncWorker::Start()
{
    uv_timer_start( &this->timer_ , 
                    SyncWorker::uv_process_timer_tick_callback , 
                    0 , 
                    this->loop_time_ );
}

NS_MARATON_END