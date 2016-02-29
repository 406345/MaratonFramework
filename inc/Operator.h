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

class Operator
{
public:

    Operator( std::string address , int port ); 

    virtual     ~Operator   ( );
    std::string IP          ( );
    std::string Address     ( );
    int         Port        ( ); 

protected:

    virtual Session * CreateSession    ( ) = 0;
    virtual void      OnSessionOpen   ( Session * session ) = 0;
    virtual void      OnSessionClose  ( Session * session ) = 0;

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
