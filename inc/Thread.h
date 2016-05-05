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

#ifndef THREAD_H_
#define THREAD_H_

#include <uv.h>
#include <functional>
#include <Macro.h>

NS_MARATON_BEGIN

using namespace std;

// The class can create a thread
class Thread
{
public:

    typedef function<void( void* argv )> thread_callback_t;

    // Default constructor
    // @cb   : callback when the thread is running
    //         void( void* argv )
    // @data : parameters pass to thread
    Thread( thread_callback_t cb , void* data);
    ~Thread();

    // Wait until the thread exits;
    void Wait();

private:

    uv_thread_t thread_;
    thread_callback_t callback_;
    void* data_ = nullptr;

    static void on_thread_callback( void* argv );
};

NS_MARATON_END

#endif // !THREAD_H_