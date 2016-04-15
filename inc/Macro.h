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
* Description   : DO NOT use DEBUG marcoes in release version!
* Creator       : shubo yang
* Date          :
* Modifed       : When      | Who       | What
***********************************************************************************/

#ifndef MRT_MACRO_H_
#define MRT_MACRO_H_

#include <memory>
#include "Logger.h"

// Cast values defination
#define MAX_CIRCLE_BUFFER_SIZE  1024 * 1024 * 20
#define MAX_PATH_LEN            1024
#define MAX_CONNECTION_SIZE     2048
#define MAX_UINT                0xFFFFFFFF

#define NS               MRT
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

#define MAKE_SINGLETON( __type__ ) static __type__ * Instance() \
{ \
    static __type__ inst_; \
    return &inst_; \
}


#define uptr std::unique_ptr
#define sptr std::shared_ptr

#define make_uptr(__t__,...) std::unique_ptr< __t__ >( new __t__(__VA_ARGS__) ) 
#define make_sptr(__t__,...) std::make_shared< __t__ >(__VA_ARGS__)
#define move_ptr(__v__) std::move(__v__)
#define scast static_cast


#endif // !MRT_MACRO_H_



