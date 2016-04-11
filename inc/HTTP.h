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

#ifndef HTTP_H_
#define HTTP_H_

#include <string>
#include <map>
#include <functional>

#include "Macro.h"
#include "Buffer.h"
#include "Connector.h"
#include "CircleBuffer.h"
#include "Maraton.h"
#include <vector>

NS_MARATON_BEGIN

using namespace std;

class HTTPRequest;
class HTTPResponse;

typedef std::function<uptr<Buffer>( HTTPRequest* req )> WriteCallbackType;
typedef std::function<uptr<Buffer>( HTTPResponse* rep )> ReadCallbackType;
typedef std::function<void( HTTPResponse* rep , uptr<Buffer> buffer )> RepReadCallbackType;

class Url
{
public:

    Url                      ( std::string url );
    ~Url                     ();
                             
    std::string Domain       ();
    std::string Path         ();
    std::string Protocol     ();
    int         Port         ();
    string      Parameter    ( string name );

protected:

    virtual void Parse( std::string url );

private:

    std::string url_      = "";
    std::string domain_   = "";
    std::string path_     = "";
    std::string protocol_ = "";
    int         port_     = 80;
    std::string tmp_      = "";

    map<string,string> parameter_;
};
// ===========================================
// HTTPRequest
// ===========================================
class HTTPRequest
{
public:
     
    HTTPRequest ( std::string url ,
                  std::string method );
    HTTPRequest ();
    ~HTTPRequest();

    void         WriteCallback  ( WriteCallbackType callback );

    void         Content        ( uptr<Buffer> content );
    void         Content        ( std::string content );
    uptr<Buffer> Content        ();
    void         ContentLength  ( size_t size );
    size_t       ContentLength  ();
    
    void         Header         ( std::string key , std::string value );
    std::string  Header         ( std::string key );
    std::string  Domain         (){ return this->domain_; };
    std::string  Host           (){ return this->ip_;     };
    std::string  RequestUrl     ();

    void Parse                  ( uptr<Buffer> data );

    uptr<Buffer> BuildHeader    ();
    uptr<Buffer> BuildBody      ();

    void*        Data           ();
    void         Data           ( void* value );

    bool         Finish         ();
    sptr<Url>    Uri            ();
     
private:

    std::string                         ip_             = "";
    int                                 port_           = 80;
    std::string                         url_            = "";
    std::string                         method_         = "";
    std::string                         domain_         = "";
    std::map<std::string , std::string> header_;
    size_t                              content_length_ = 0;
    uptr<Buffer>                        content_        = nullptr;
    WriteCallbackType                   write_callback_ = nullptr;
    void*                               data_           = nullptr;

    // Parsing used
    // ==================================================================
    enum ParseState
    {
        kMethod = 1,
        kUrl,
        kProtocol,
        kHeadKey,
        kHeadValue,
        kContent
    };

    ParseState  parse_state_ = ParseState::kMethod;
    std::string tmp_key_     = "";
    std::string tmp_value_   = "";
    std::string protocol_    = "";

    friend class WebClient;
};

// ===========================================
// HTTPResponse
// ===========================================
class HTTPResponse
{
public:
     
    //HTTPResponse                 ( size_t Status );
    HTTPResponse                 ();
    ~HTTPResponse                ();

    void         ReadCallback    ( RepReadCallbackType callback );
                                 
    void*        Data            ();
    void         Data            ( void* value );
                                        
    void         Header          ( std::string key , 
                                   std::string value );
    std::string  Header          ( std::string key );
                 
    void         Content         ( uptr<Buffer> content );
    void         Content         ( std::string content );
    uptr<Buffer> Content         ();
    void         ContentLength   ( size_t size );
    size_t       ContentLength   ();

    void         Status          ( size_t code );
    size_t       Status          ();

    uptr<Buffer> BuildHeader     ();
    uptr<Buffer> BuildBody       ();

    void         Parse           ( uptr<Buffer> data );

    bool         Finish          ();

private: 

    size_t              status_         = 0;
    std::string         status_str_     = "";
    size_t              content_length_ = 0;
    std::string         protocol_       = "";
    RepReadCallbackType read_callback_  = nullptr;
    void*               data_           = nullptr;
    uptr<Buffer>        content_        = nullptr;
    std::map<
       std::string , 
       std::string>     header_;

    // Parsing used
    // ==================================================================
    enum ParseState
    {
        kProtocol = 1,
        kStatusCode,
        kStatus,
        kHeadKey,
        kHeadValue,
        kContent
    };
   
    ParseState   parse_state_ = ParseState::kProtocol;
    std::string  tmp_key_     = "";
    std::string  tmp_value_   = "";
}; 

class WebRequestSession :
    public Session
{
public:

    WebRequestSession           ( uptr<HTTPRequest> req );
    ~WebRequestSession          ( );

    uptr<HTTPResponse> Response ( );

protected:

    virtual void OnConnect      ()                  ;
    virtual void OnRead         ( uptr<Buffer> data );
    virtual void OnWrite        ( uptr<Buffer> data );
    virtual void OnClose        ()                  ;

private:

    uptr<HTTPRequest>       req_;
    uptr<HTTPResponse>      rep_;
};

NS_MARATON_END

#endif // !HTTP_H_