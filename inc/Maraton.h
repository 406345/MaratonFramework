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

#ifndef MARATON_H_
#define MARATON_H_

#include <functional>
#include <vector>

#include "Macro.h"
#include "Operator.h"
#include "Listener.h"
#include "Connector.h"

NS_MARATON_BEGIN

class Maraton
{
public:

    static Maraton* Instance( )
    {
        static Maraton* inst = nullptr;

        if ( inst == nullptr )
        {
            static Maraton maraton;
            inst = &maraton;
        }

        return inst;
    }

    void Regist     ( sptr<Operator> listener );
    void Unregist   ( sptr<Operator> opt );
    void Unregist   ( const Operator * opt );
    void Run        ();

private:

    Maraton( )
    {
    };

    ~Maraton( )
    {
    };

    uv_loop_t*          uv_loop( );
    sptr<Operator>      elements_[MAX_CONNECTION_SIZE];
    int                 elements_index_                 = 0;
     
    static void uv_process_resolved( uv_getaddrinfo_t * req , int status , addrinfo * res );
};

NS_MARATON_END

#endif // !MARATON_H_
