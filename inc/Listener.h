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

#ifndef MRT_LISTENER_H_
#define MRT_LISTENER_H_

#include <string>

#include "Macro.h"
#include "Operator.h"

NS_MARATON_BEGIN

// @Description : Base class for a listener
//                Listener will listen on the port, wait for connections 
//                and never exit.
class Listener :
    public Operator
{
public:

    // Default constructure 
    // @addr : ip or uri address
    // @port : listening port
    Listener    ( std::string addr , 
                  int port );

    ~Listener   ( );

protected:

    void              DoWork           ( ) override;

    // Close a session
    // @session : session to close
    void              CloseSession     ( Session * session ) override;

private:

    uv_tcp_t uv_tcp_ = { 0 }; 

    Session* session_list_[MAX_CONNECTION_SIZE] = { 0 };
    size_t   session_list_index_                = 0;

    static void uv_new_connection_callback ( uv_stream_t * server ,
                                             int status );
    static void uv_alloc_callback          ( uv_handle_t * handle , 
                                             size_t suggested_size , 
                                             uv_buf_t * buf );
    static void uv_read_callback           ( uv_stream_t * stream , 
                                             ssize_t nread , 
                                             const uv_buf_t * buf );
    static void uv_close_callback          ( uv_handle_t * handle );
};

NS_MARATON_END

#endif // !MRT_LISTENER_H_
