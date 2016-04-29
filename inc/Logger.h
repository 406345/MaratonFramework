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

    // Print message with sys channel
    // @fmt  : format, using % to indicate a parameter, not %d or something else.
    // @args : parameter values
    template<typename ...Types>
    static void Sys( const char* fmt , Types... args )
    {
        Tprintf( std::cout , "% [SYSTEM] " , Timer::Date() );
        Tprintf( std::cout , fmt , args... );
        Tprintf( std::cout , "\r\n" );
    }

    // Print message with error channel
    // @fmt  : format, using % to indicate a parameter, not %d or something else.
    // @args : parameter values
    template<typename ...Types>
    static void Error( const char* fmt , Types... args )
    {
        Tprintf( std::cerr , "% [ERROR] " , Timer::Date() );
        Tprintf( std::cerr , fmt , args... );
        Tprintf( std::cerr ,"\r\n" );
    }

    // Print message with normal channel
    // @fmt  : format, using % to indicate a parameter, not %d or something else.
    // @args : parameter values
    template<typename ...Types>
    static void Log( const char* fmt , Types... args )
    {
        Tprintf( std::cout , "% [INFO] " , Timer::Date() );
        Tprintf( std::cout , fmt , args... );
        Tprintf( std::cout , "\r\n" );
    }

    // Print message with debug channel
    // !!!! This channel will be disabled in release version
    // @fmt  : format, using % to indicate a parameter, not %d or something else.
    // @args : parameter values
    template<typename ...Types>
    static void Debug( const char* fmt , Types... args )
    {
#ifdef _DEBUG
        Tprintf( std::cout ,"% [DEBUG] " , Timer::Date() );
        Tprintf( std::cout , fmt , args... );
        Tprintf( std::cout ,"\r\n" );
#endif
    }


private:

    Logger()
    {
    };
    ~Logger()
    {
    };

    static void Tprintf( std::ostream & stream , const char* format ) // base function
    {
        stream << format;
    }

    template<typename T , typename... Targs>
    static void Tprintf( std::ostream & stream , const char* format , T value , Targs... Fargs ) // recursive variadic function
    {
        for ( ; *format != '\0'; format++ )
        {
            if ( *format == '%' )
            {
                stream << value;
                // recursive call;
                return Tprintf( stream , format + 1 , Fargs... ); 
            }
            stream << *format;
        }
    }
};

#endif // !MRT_LOGER_H_ 