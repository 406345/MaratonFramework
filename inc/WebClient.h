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

#ifndef WEB_CLIENT_H_
#define WEB_CLIENT_H_

#include <string>
#include <map>

#include "uv.h"
#include "HTTP.h"

NS_MARATON_BEGIN 

typedef std::function<void( uptr<HTTPResponse> )>  CallbackResponseType;
 
class WebClient
{
public:

    WebClient ( );

    void Get        ( std::string url , 
                      CallbackResponseType callback );

    void Post       ( std::string url , 
                      std::string data , 
                      CallbackResponseType callback ); 

    void PostFile   ( std::string url , 
                      std::string file_token , 
                      FILE* pfile , 
                      CallbackResponseType callback );

    void DownloadFile ( std::string url ,
                        FILE* pfile ,
                        CallbackResponseType callback);

    uptr<HTTPResponse> GetSync  ( std::string url );
                       
    uptr<HTTPResponse> PostSync ( std::string url , 
                                  std::string data);

    void Header( std::string key, std::string value );

private:

    class WebClientRequestToken
    {
    public:

        uptr<HTTPRequest>    req_             = nullptr;
        uptr<HTTPResponse>   rep_             = nullptr;
        CallbackResponseType callback_        = nullptr; 

        std::string       ip_                = "";
        std::string       address_            = "";
        int               port_               = 80;
        uv_loop_t*        uv_loop            = nullptr;
        uv_getaddrinfo_t  uv_getaddrinfo     = { 0 };
        addrinfo          addrinfo_           = { 0 };
        sockaddr_in       addr_in_            = { 0 };
                                            
        uv_connect_t      uv_connect         = { 0 };
        uv_tcp_t          uv_tcp             = { 0 };

        void InvokeCallback( uptr<HTTPResponse> rep )
        {
            if ( this->callback_ != nullptr )
            {
                this->callback_( move_ptr( rep ) );
            }
        }
    };

    struct write_token_t
    {
        uv_write_t *                writer  = nullptr;
        uv_buf_t *                  buffer  = nullptr;
        WebClientRequestToken *     session = nullptr;
    };

    void QueryDns                      ( uptr<WebClientRequestToken> token );
    void FillHeader                    ( HTTPRequest* req );

    static void uv_send_request         ( WebClientRequestToken* token );
    static void uv_process_resolved     ( uv_getaddrinfo_t * req , 
                                          int status , 
                                          addrinfo * res );
    static void uv_connected_callback   ( uv_connect_t* req , 
                                          int status );
    static void uv_alloc_callback       ( uv_handle_t * handle , 
                                          size_t suggested_size , 
                                          uv_buf_t * buf );
    static void uv_read_callback        ( uv_stream_t * stream , 
                                          ssize_t nread , 
                                          const uv_buf_t * buf );
    static void uv_write_callback       ( uv_write_t * req , 
                                          int status );
    static void uv_close_callback       ( uv_handle_t* handle );

    std::map<std::string , std::string> header_;
    uv_loop_t                           uv_loop_ = { 0 };
};

NS_MARATON_END

#endif // !WEB_CLIENT_H_