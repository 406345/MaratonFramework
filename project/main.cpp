///***********************************************************************************
//This file is part of Project for MaratonFramework
//For the latest info, see  https://github.com/Yhgenomics/MaratonFramework.git
//
//Copyright 2016 Yhgenomics
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.
//***********************************************************************************/
//
///***********************************************************************************
//* Description   :
//* Creator       :
//* Date          :
//* Modifed       : When      | Who       | What
//***********************************************************************************/
//
//#include <chrono>
//#include "MRT.h"
//#include <thread>
//#include <ostream>
//#include <string>
//#include <Manager.h>
//
//using namespace MRT;
//using namespace std;
//
//uv_thread_t pthread;
//
//class MasterConnector :
//    public Connector
//{
//public:
//
//    MasterConnector ( string ip , int port )
//        : Connector( ip , port )
//    {
//
//    }
//    ~MasterConnector()
//    {
//
//    }
//
//protected:
//
//    virtual Session * CreateSession    () override
//    {
//        return new Session();
//    };
//
//    // Callback when a session is created
//    virtual void      OnSessionOpen   ( Session * session ) override
//    {
//
//    };
//
//    // Callback after a session is closed
//    virtual void      OnSessionClose  ( Session * session ) override
//    {
//        SAFE_DELETE( session );
//    };
//};
//
//int t = 0;
//int main( int argc , char * argv[] )
//{
//    Thread thr( [ ] ( void*data )
//    {
//        while ( true )
//        {
//            Maraton::Instance()->Run();
//        }
//    } , nullptr );
//
//    scanf( "%d" , &t );
//
//    Syncer sync( Maraton::Instance()->Event() , [ ] ()
//    {
//        auto cnn = make_sptr( MasterConnector , "10.0.0.11" , 112 );
//        Maraton::Instance()->Regist( cnn );
//        Logger::Log( "async_cb" );
//
//    } );
//
//    scanf( "%d" , &t );
//    return 0;
//}