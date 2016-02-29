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

#ifndef SYNC_WORKER_H_
#define SYNC_WORKER_H_

#include <functional>

#include "uv.h"
#include "Macro.h"

NS_MARATON_BEGIN

class SyncWorker
{
public:

    typedef std::function<bool( SyncWorker* )> SyncworkerCallbackType;

    static SyncWorker* Create       ( const SyncworkerCallbackType  work_callback , 
                                      const SyncworkerCallbackType  after_callback , 
                                      void* data );

    static SyncWorker* Create       ( const size_t time_span,
                                      const SyncworkerCallbackType  work_callback , 
                                      const SyncworkerCallbackType  after_callback , 
                                      void* data );
    static void        Stop         ( SyncWorker * worker );

    void*              Data         () { return this->data_; };
    void               Data         ( void* value ) { this->data_ = value; };
    size_t             LoopCount    () { return loop_count_; };
    size_t             LoopTime     () { return loop_time_ * loop_count_; };

private:

    static void uv_process_timer_tick_callback( uv_timer_t* handle );

    SyncWorker  ( );
    ~SyncWorker ( );

    void Stop   ( );
    void Start  ( );

    bool                            finished_       = false;
    void*                           data_           = nullptr;
    uv_timer_t                      timer_          = { 0 };
    size_t                          loop_time_      = 1;

    SyncworkerCallbackType           cb_work_        = nullptr;
    SyncworkerCallbackType           cb_after_work_  = nullptr;
    size_t                          loop_count_     = 1;
};

NS_MARATON_END

#endif // !SYNC_WORKER_H_