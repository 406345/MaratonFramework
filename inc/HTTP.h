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
    std::string PathWithArg  ();
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

    map<string , string> parameter_;

    std::string UrlEncode( const std::string& szToEncode )
    { 
        std::string src = szToEncode;
        char hex[] = "0123456789ABCDEF";
        string dst;
        for ( size_t i = 0; i < src.size(); ++i )
        {
            unsigned char cc = src[i];
            if ( isascii( cc ) )
            {
                if ( cc == ' ' )
                {
                    dst += "%20";

                }
                else
                    dst += cc;
            }
            else
            {

                unsigned char c = static_cast< unsigned char >( src[i] );
                dst += '%';
                dst += hex[c / 16];
                dst += hex[c % 16];
            }

        }
        return dst;
    }
    std::string UrlDecode( const std::string& szToDecode )
    {
        std::string result;
        int hex = 0;
        for ( size_t i = 0; i < szToDecode.length(); ++i )
        {
            switch ( szToDecode[i] )
            {
                case '+':
                    result += ' ';
                    break;
                case '%':
                    if ( isxdigit( szToDecode[i + 1] ) && isxdigit( szToDecode[i + 2] ) )
                    {
                        std::string hexStr = szToDecode.substr( i + 1 , 2 );
                        hex = strtol( hexStr.c_str() , 0 , 16 );
                        //字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@] 

                        //可以不经过编码直接用于URL  
                        if ( !( ( hex >= 48 && hex <= 57 ) || //0-9  
                             ( hex >= 97 && hex <= 122 ) ||   //a-z  
                             ( hex >= 65 && hex <= 90 ) ||    //A-Z  
                             //一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]  
                             hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29
                             || hex == 0x2a || hex == 0x2b || hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f
                             || hex == 0x3A || hex == 0x3B || hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f
                             ) )
                        {
                            result += char( hex );
                            i += 2;
                        }
                        else result += '%';
                    }
                    else
                    {
                        result += '%';
                    }
                    break;
                default:
                    result += szToDecode[i];
                    break;
            }

        }
        return result;
    }
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
    std::string  Domain         ()
    {
        return this->domain_;
    };
    std::string  Host           ()
    {
        return this->ip_;
    };
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
        kMethod = 1 ,
        kUrl ,
        kProtocol ,
        kHeadKey ,
        kHeadValue ,
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
        kProtocol = 1 ,
        kStatusCode ,
        kStatus ,
        kHeadKey ,
        kHeadValue ,
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
    ~WebRequestSession          ();

    uptr<HTTPResponse> Response ();

protected:

    virtual void OnConnect      ();
    virtual void OnRead         ( uptr<Buffer> data );
    virtual void OnWrite        ( uptr<Buffer> data );
    virtual void OnClose        ();

private:

    uptr<HTTPRequest>       req_;
    uptr<HTTPResponse>      rep_;
};

NS_MARATON_END

#endif // !HTTP_H_