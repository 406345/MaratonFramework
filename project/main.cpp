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
//int main( )
//{
//
//    MRT::FileSystem fs( Maraton::Instance( )->Event( ) );
//    fs.Open( "D:\\AAA.txt" , [ ] ( long long status , FileSystem* fs )
//    {
//        if ( status < 0 )
//        {
//            Logger::Log( "open error  %" , fs->ErrorStr(status) );
//            return;
//        }
//        auto size = fs->Tell( );
//        Logger::Log( "file size %" , size );
//
//        fs->Read( 2 , [ ] ( long long status , FileSystem* fs , sptr<Buffer> buf )
//        {
//            if ( status > 0 )
//            {
//                Logger::Log( "file  %" , buf->Data() );
//            }
//        } );
//
//        fs->Read( 2 , [ ] ( long long status , FileSystem* fs , sptr<Buffer> buf )
//        {
//           if ( status > 0 )
//            {
//                Logger::Log( "file  %" , buf->Data() );
//            }
//        } );
//
//        fs->Read( 2 , [ ] ( long long status , FileSystem* fs , sptr<Buffer> buf )
//        {
//            if ( status > 0 )
//            {
//                Logger::Log( "file  %" , buf->Data() );
//            }
//        } );
//
//        fs->Write( make_sptr( Buffer , "XXXX" ) , [ ] ( long long status , FileSystem* fs)
//        { 
//            Logger::Log( "write file result  %" , status );
//        } );
//    } );
//
//    Maraton::Instance( )->Run( );
//
//    return 0;
//}