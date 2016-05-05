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

#ifndef SYNCER_H_
#define SYNCER_H_

#include <uv.h>
#include <functional>
#include <Macro.h>
#include <LoopEvent.h>

using namespace std;

NS_MARATON_BEGIN

// Sync thread
class Syncer
{
public:

    typedef function<void( void )> syncer_callback_t;

    // Default constructor
    // @loop : the eventloop to sync
    // @cb   : callback when the thread is ready
    //         function<void( void )> syncer_callback_t
    Syncer( sptr<LoopEvent> loop , syncer_callback_t cb );

private:

    sptr<LoopEvent>   loop_;
    syncer_callback_t callback_;
    uv_async_t        async_ = { 0 };

    static void async_cb( uv_async_t* handle );
};

NS_MARATON_END

#endif // !SYNCER_H_