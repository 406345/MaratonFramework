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
//int main( int argc , char * argv[] )
//{
//    MRT::WebClient wc;
//    wc.Get( "http://10.0.0.234:80/v1/file?path=/fastq/GCP15_GTACGC_L002_R1_001.fastq" , [ ] (uptr<HTTPResponse> rep)
//    {
//        if ( rep->Finish() && rep->ContentLength() > 0)
//        {
//               auto content = rep->Content();
//               printf( "%s" ,  content->Data() );
//        }
//    } );
//
//    Maraton::Instance( )->Run( );
//
//    return 0;
//}