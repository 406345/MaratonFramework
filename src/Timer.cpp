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

#include "SystemInfo.h"
#include "Timer.h"


size_t Timer::Tick()
{
    return MRT::SystemInfo::Time( );
};

string Timer::Date()
{
    const time_t t = time(NULL);
    //time(&timep);
    struct tm* current_time = localtime(&t);
  
    char timer_str[512] = { 0 };

    sprintf( timer_str , 
             "%d-%d-%d %d:%d:%d" , 
             current_time->tm_year + 1900,
             current_time->tm_mon + 1,
             current_time->tm_mday,
             current_time->tm_hour,
             current_time->tm_min,
             current_time->tm_sec);
    string ret = string( timer_str );

    return ret;
};
