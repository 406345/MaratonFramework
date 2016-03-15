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

#ifndef MRT_CONNECTOR_H_
#define MRT_CONNECTOR_H_

#include <string>

#include "Macro.h"
#include "Operator.h"

NS_MARATON_BEGIN

// @Description : Base class for connector. 
//                It is used for connector to remote server with given ip and port.
//                Instance must be register by Maraton::Instance()->Regist() to run
class Connector :
    public Operator
{
public:

    Connector    ( std::string address,
                   int port );
    ~Connector   ();
                 
protected:        

    void              DoWork           () override;
    void              CloseSession     ( Session * session ) override;

private:

    uv_connect_t      uv_connect_          = { 0 };
    uv_tcp_t          uv_tcp_              = { 0 };

    static void uv_connected_callback   ( uv_connect_t* req , 
                                          int status );
    static void uv_alloc_callback       ( uv_handle_t * handle , 
                                          size_t suggested_size , 
                                          uv_buf_t * buf );
    static void uv_read_callback        ( uv_stream_t * stream , 
                                          ssize_t nread , 
                                          const uv_buf_t * buf );
    static void uv_close_callback       ( uv_handle_t* handle );

    Session*    session_ = nullptr;

    friend class Session;
};

NS_MARATON_END

#endif // !MRT_CONNECTOR_H_
