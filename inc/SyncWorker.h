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

// @Description : Help create a syncwork in main thread
//                The syncwork is in main thread , so it can block the
//                thread. DON'T run any heavy work in it!
class SyncWorker
{
public:

    typedef std::function<bool( SyncWorker* )> SyncworkerCallbackType;

    // Create a syncworker, the worker will callback every 1 ms
    // @work_callback  : callback, return true if work is done
    // @after_callback : callback, return true if work is done
    // @data           : data to input
    static SyncWorker* Create       ( const SyncworkerCallbackType  work_callback , 
                                      const SyncworkerCallbackType  after_callback , 
                                      void* data );

    // Create a syncworker, the worker will callback every [time_span] ms
    // @time_span      : loop time duration
    // @work_callback  : callback, return true if work is done
    // @after_callback : callback, return true if work is done
    // @data           : data to input
    static SyncWorker* Create       ( const size_t time_span,
                                      const SyncworkerCallbackType  work_callback , 
                                      const SyncworkerCallbackType  after_callback , 
                                      void* data );

    // Stop the worker
    // @worker : worker to be stopped
    static void        Stop         ( SyncWorker * worker );

    // Get the data 
    void*              Data         () { return this->data_; };

    // Set the data
    // @value : data to be set
    void               Data         ( void* value ) { this->data_ = value; };

    // Get the loop count
    size_t             LoopCount    () { return loop_count_; };
    
    // Get the loop duration
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