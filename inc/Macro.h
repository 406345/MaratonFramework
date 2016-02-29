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

#ifndef MRT_MACRO_H_
#define MRT_MACRO_H_

#include <memory>
#include "Logger.h"

// Cast values defination
#define MAX_CIRCLE_BUFFER_SIZE  1024*1024*5
#define MAX_PATH_LEN            1024
#define MAX_CONNECTION_SIZE     2048
#define MAX_UINT                0xFFFFFFFF


#define NS                  MRT
#define NS_MARATON_BEGIN namespace NS {
#define NS_MARATON_END }

// Macro function definitions
#define PP_DEF(__type__,__name__)                           \
    void __name__(__type__ value){ __name__##_ = value; };  \
    __type__ __name__(){ return __name__##_; };

#define SAFE_DELETE(__x__)                                  \
    if ( nullptr != __x__ )                                 \
    {                                                       \
        delete __x__;                                       \
        __x__ = nullptr;                                    \
    }

#define SAFE_DELETE_ARRAY(__x__)                            \
    if ( nullptr != __x__ )                                 \
    {                                                       \
        delete[] __x__;                                     \
        __x__ = nullptr;                                    \
    }

#define SINGLETON_BEGIN(__class__)                          \
class __class__                                             \
{                                                           \
private:                                                    \
    __class__(){};                                          \
    ~__class__(){};                                         \
public:                                                     \
    static __class__ * instance()                           \
    {                                                       \
        static __class__ * pT = nullptr;                    \
        if ( !pT )                                          \
        {                                                   \
            static __class__ instance;                      \
            pT = &instance;                                 \
        }                                                   \
        return pT;                                          \
    };

#define SINGLETON_END }; 

#define uptr std::unique_ptr
#define sptr std::shared_ptr

#define make_uptr(__t__,...) std::unique_ptr< __t__ >( new __t__(__VA_ARGS__) ) 
#define make_sptr(__t__,...) std::make_shared< __t__ >(__VA_ARGS__)
#define move_ptr(__v__) std::move(__v__)
#define scast static_cast

// Debug mode definitions
#define DEBUG_MODE 1
#if DEBUG_MODE

#if _WIN32

#define LOG_DEBUG(msg_,...) Logger::Log("%s:%d "##msg_,__FILE__,__LINE__,##__VA_ARGS__)
#define LOG_DEBUG_UV(status) \
                if( status != 0 ) \
                    Logger::Log("%s:%d %s",\
                                __FILE__,\
                                __LINE__,\
                                uv_strerror((int)status))

#else
#define LOG_DEBUG  
#define LOG_DEBUG_UV(status) 

#endif

#else
#define LOG_DEBUG  
#define LOG_DEBUG_UV(status)
#endif

#if _WIN32

#define LOG_UV_ERROR(__x__) if ( __x__ != 0 ) printf( "error %s", \
                                                  uv_strerror((int)__x__));
#define LOG_SYS(msg_,...) Logger::Sys(msg_,__VA_ARGS__)
#define LOG_EERROR(msg_,...) Logger::Error(msg_,__VA_ARGS__)
#define UV_ERROR(status) uv_strerror((int)status)

#else
#define LOG_UV_ERROR(__x__)  
#define LOG_SYS 
#define LOG_EERROR 
#define UV_ERROR(status) 

#endif
 
#endif // !MRT_MACRO_H_



