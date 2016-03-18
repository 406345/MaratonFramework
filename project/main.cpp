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

#include <chrono>
#include "MRT.h"
#include <thread>
#include <ostream>
#include <string>
#include <Manager.h>

using namespace MRT;
using namespace std;

int main( )
{

    auto worker = MRT::AsyncWorker::Create([]( AsyncWorker* worker)
    { 
        
        uv_loop_t loop;
        uv_loop_init( &loop );

        while(true)
        {
            uv_run(&loop,UV_RUN_DEFAULT);
            std::this_thread::sleep_for( std::chrono::milliseconds(1));
        }

    },nullptr);

    MRT::Manager<MRT::Buffer> mgr;
    WebClient client;

    client.GetX( "http://www.baidu.com" , [ ] ( HTTPResponse * rep , uptr<Buffer> buf )
    { 
        printf( buf->Data() );
        
    } );

    while ( true )
    {
        MRT::Maraton::Instance( )->Run( );
    }
    //Logger::Log( ret->Content( )->Data( ) );

    //auto ret2 = client.PostSync( "http://www.baidu.com" , "" );
    //Logger::Log( ret2->Content( )->Data( ) );

    return 0;
}