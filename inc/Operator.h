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

#ifndef MRT_OPERATOR_H_
#define MRT_OPERATOR_H_

#include <string>

#include "Macro.h"
#include "uv.h"
#include "Session.h"

NS_MARATON_BEGIN

// @Description : Base class for Connector and Listener.
class Operator
{
public:

    // Default Constructure
    // @address : hostname, ip address
    // @port    : connect or listen port
    Operator( std::string address , int port ); 

    virtual     ~Operator   ( );

    // Get ip address
    std::string IP          ( );

    // Get ip or uri address
    std::string Address     ( );

    // Get port
    int         Port        ( ); 

protected:

    // Create a session 
    virtual Session * CreateSession    ( ) = 0;

    // Callback when a session is created
    virtual void      OnSessionOpen   ( Session * session ) = 0;

    // Callback after a session is closed
    virtual void      OnSessionClose  ( Session * session ) = 0;

    // Close a session
    virtual void      CloseSession     ( Session* session ) = 0;
    virtual void      DoWork           ( ) = 0;

private:
    
    std::string       ip_              = "";
    std::string       address_         = "";
    int               port_            = 0; 
    uv_loop_t*        uv_loop_         = nullptr;
    uv_getaddrinfo_t  uv_getaddrinfo_  = { 0 };
    addrinfo          addrinfo_        = { 0 };
    sockaddr_in       addr_in_         = { 0 };
    size_t            index_           = 0 ;
     
    friend class Maraton;
    friend class Listener;
    friend class Connector;
    friend class Session;
};

NS_MARATON_END

#endif // !MRT_OPERATOR_H_
