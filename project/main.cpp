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
#include <sstream>

using namespace MRT;
using namespace std;

uv_thread_t pthread;

void run()
{
    bool cnti = false;
    MRT::WebClient client;
    client.Post( "http://10.0.0.55:4243/images/create?fromImage=10.0.0.15:5000/yhfsdl" ,
                 "",
                [ &client ] ( uptr<MRT::HTTPResponse> rep )
    {
        if ( rep == nullptr)
        {
            Logger::Log( "rep is nullptr !!!!!" );
            return;
        }

        if ( rep->Content() == nullptr )
        {
            Logger::Log( "rep content is nullptr !!!!" );
            return;
        }

        Logger::Log( "data size: %" , rep->Content()->Size() );
        Logger::Log( "data     : %" , rep->Content()->Data() );
        return  run();
    } );
     
    std::this_thread::sleep_for( std::chrono::milliseconds(10 ) );
}
 
int main( int argc , char * argv[] )
{
    auto t = SyncWorker::Create( 100 , [ ] ( SyncWorker* worker )
    {
        MRT::WebClient client;
        client.Post( "http://10.0.0.55:4243/images/create?fromImage=10.0.0.15:5000/yhfsdl" ,
                     "" ,
                     [&client] ( uptr<MRT::HTTPResponse> rep )
        {
            if ( rep == nullptr )
            {
                Logger::Log( "rep is nullptr !!!!!" );
                return;
            }

            if ( rep->Content() == nullptr )
            {
                Logger::Log( "rep content is nullptr !!!!" );
                return;
            }

            Logger::Log( "data size: %" , rep->Content()->Size() );
            Logger::Log( "data     : %" , rep->Content()->Data() );
        } );
        return false;

    } , nullptr, nullptr );


    MRT::Maraton::Instance()->Run();
    scanf( "" );
    return 0;
}