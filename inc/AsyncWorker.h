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

#ifndef ASYNC_WORKER_H_
#define ASYNC_WORKER_H_ 

#include <functional>

#include "Macro.h"
#include "uv.h"

NS_MARATON_BEGIN

class AsyncWorker;

class IAsyncWorkerNotifier
{
public:
    virtual void OnAsyncWork( AsyncWorker* worker ) {};
    virtual void OnAsyncWorkFinish( AsyncWorker* worker ) {};
};

class AsyncWorker
{
public:

    typedef std::function<void( AsyncWorker* )> CallbackType;

    static AsyncWorker* Create( CallbackType acting ,
                                CallbackType finish ,
                                void* data );

    static AsyncWorker* Create( CallbackType acting ,
                                void* data );

    static void         Stop( AsyncWorker* worker );


    void  Data( void* value ) { this->data_ = value; };
    void* Data() { return this->data_; };


private:

    void Stop();
    AsyncWorker( CallbackType acting , CallbackType finish );

    void Start();

    void*       data_;
    uv_work_t   worker_;
    CallbackType  acting_callback_;
    CallbackType  finish_callback_;

    static void uv_process_work_callback( uv_work_t* req );
    static void uv_process_after_work_callback( uv_work_t* req , int status );
};

NS_MARATON_END

#endif // !ASYNC_WORKER_H_ 
