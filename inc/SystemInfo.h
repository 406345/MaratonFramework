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

#ifndef SYSETEM_INFO_H_
#define SYSETEM_INFO_H_

#include <string>
#include <vector>
#include <Macro.h>
#include <uv.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#endif

NS_MARATON_BEGIN

using std::vector;
using std::string;

// @Description : Helpe get the system information
class SystemInfo
{
public:


    // Get the total memory size
    static size_t MemorySize( )
    {
        return uv_get_total_memory( );
    };

    // Get the free memory size
    static size_t MemoryFreeSize( )
    {
        return uv_get_free_memory( );
    };

    // Get the cpu core number
    static int CPUNum( )
    {
        int cpu_count            = 0;
        uv_cpu_info_t * cpu_info = { 0 };

        uv_cpu_info     ( &cpu_info , &cpu_count );
        uv_free_cpu_info( cpu_info  , cpu_count );
        
        return cpu_count;
    };

    // Get the cpu speed
    // @id : cpu index
    static int CPUSpeed( int id )
    {
        int cpu_speed            = 0;
        int cpu_count            = 0;
        int result               = 0;
        uv_cpu_info_t * cpu_info = { 0 };

        result = uv_cpu_info( &cpu_info , &cpu_count );

        if ( result < 0 )
        {
            //LOG_DEBUG_UV( result );
            return result;
        }

        if ( id >= cpu_count )
            return -1;

        cpu_speed = cpu_info[id].speed;

        uv_free_cpu_info( cpu_info , cpu_count );

        return cpu_speed;
    };

    // Get the current work directory
    static std::string CurrentDirectory( )
    {
        char    path[1024] = { 0 };
        size_t  path_len   = 0;

        uv_cwd( path , &path_len );

        return std::string( path , path_len );
    };

    // Get the current time in microsecond 
    // This function may not work in some version of windows
    static size_t Time( )
    {
        return ( uv_hrtime( ) / 1000 );
    };

    // Get all the IP addresses
    static vector<string> IPAddres()
    {
        vector<string> ret;
        int count = 0;
        uv_interface_address_t* addrs = nullptr;
        uv_interface_addresses( &addrs , &count );

        for ( int i = 0; i < count; i++ )
        {
            char ipv4[64] = { 0 };
            uv_ip4_name( &addrs[i].address.address4 , ipv4 , 64 );

            if ( ipv4[0] != 0 )
            {
                ret.push_back( string( ipv4 ) );
            }
        }

        uv_free_interface_addresses( addrs , count );
        return vector<string>( ret ); 
    }

    //Get the host name
    static string HostName()
    {
        char name[128] = { 0 };
        gethostname( name , 128 );
        
        return string( name );
    }

};

NS_MARATON_END

#endif // !SYSETEM_INFO_H_
