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
#include "LoopEvent.h"

NS_MARATON_BEGIN

// @Description : Singleton class.
//                Main class for then entire framework
//                Every operator( Connector or Listener ) must be register
//                Run must be invoked to 
//                startup the dispatcher thread to dispatch messages and event
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

    // Register a operator
    // @opt : subclass of Operator
    void Regist     ( sptr<Operator> opt );

    // Unregister a operator
    // @opt : subclass of Operator
    void Unregist   ( sptr<Operator> opt );

    // Unregister a operator
    // @opt : subclass of Operator
    void Unregist   ( const Operator * opt );

    // Run the main loop
    // this method will block the thread.
    // Return if there is no more event to handle
    void Run        ();

    // Get the loop event
    sptr<LoopEvent> Event() { return this->loop_event; }

private:

    Maraton( )
    {
        loop_event = make_sptr( LoopEvent );
    };

    ~Maraton( )
    {
    };

    uv_loop_t*          uv_loop( );
    sptr<Operator>      elements_[MAX_CONNECTION_SIZE];
    sptr<LoopEvent>     loop_event;
    int                 elements_index_                 = 0;
     
    static void uv_process_resolved( uv_getaddrinfo_t * req , int status , addrinfo * res );
};

NS_MARATON_END

#endif // !MARATON_H_
