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

#ifndef MRT_LOGER_H_
#define MRT_LOGER_H_ 

#include <stdio.h>
#include <iostream>
#include <stdarg.h>
#include <time.h>
#include "Timer.h"

// @Description : static class, print message to console
class Logger
{
public:

    // print message with sys channel
    // @fmt  : format, using % to indicate a parameter, not %d or something else.
    // @args : parameter values
    template<typename ...Types>
    static void Sys( const char* fmt, Types... args)
    {
#ifdef _WIN32
        Tprintf( "[%] ", Timer::Date() );
#else
        Tprintf( "\033[1;33m[%] ", Timer::Date() );
#endif

        Tprintf( fmt, args... );

#ifdef _WIN32
        Tprintf( "\r\n" );
#else
        Tprintf( "\033[0m\r\n" );
#endif      

    }

    // print message with error channel
    // @fmt  : format, using % to indicate a parameter, not %d or something else.
    // @args : parameter values
    template<typename ...Types>
    static void Error( const char* fmt, Types... args )
    { 
#ifdef _WIN32
        Tprintf( "[%] ", Timer::Date() );
#else
        Tprintf( "\033[0;31m[%] ", Timer::Date() );
#endif
        Tprintf( fmt , args... );

#ifdef _WIN32
        Tprintf( "\r\n" );
#else
        Tprintf( "\033[0;31m\r\n" );
#endif    
    }

    // print message with normal channel
    // @fmt  : format, using % to indicate a parameter, not %d or something else.
    // @args : parameter values
    template<typename ...Types>
    static void Log( const char* fmt, Types... args )
    { 
        Tprintf( "[%] ", Timer::Date() );
        Tprintf( fmt , args... );
        Tprintf( "\r\n" );
    }

private:

    Logger( )
    {
    };
    ~Logger( )
    {
    };

    static void Tprintf( const char* format ) // base function
    {
        std::cout << format;
    }

    template<typename T , typename... Targs>
    static void Tprintf( const char* format , T value , Targs... Fargs ) // recursive variadic function
    {
        for ( ; *format != '\0'; format++ )
        {
            if ( *format == '%' )
            {
                std::cout << value;
                Tprintf( format + 1 , Fargs... ); // recursive call
                return;
            }
            std::cout << *format;
        }
    }
};

#endif // !MRT_LOGER_H_ 