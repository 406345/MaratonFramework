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

#include "HTTP.h"

NS_MARATON_BEGIN

Url::Url( std::string url )
{
    this->url_ = url;
    this->Parse( this->url_ );
}

Url::~Url()
{

}

std::string Url::Domain()
{
    return this->domain_;
}

std::string Url::Path()
{
    return this->path_;
}

std::string Url::PathWithArg()
{
    string ret = this->path_ + "?";

    for ( auto & kv : this->parameter_ )
    {
        ret += kv.first + "=" + kv.second + "&";
    }

    return ret;
}

std::string Url::Protocol()
{
    return this->protocol_;
}

int Url::Port()
{
    return this->port_;
}

string Url::Parameter( string name )
{
    return this->parameter_[name];
}

void Url::Parse( std::string purl )
{
    std::string url = this->UrlDecode( purl );
    // http://blog.csdn.net/is2120/article/details/6251412
    int state = 0;
    int index = 0;
    string parameter_value = "";

    if ( url[0] == '/' )
    {
        state = 3;
    }

    do
    {
        switch ( state )
        {
            // Read protocol
            case 0:
                {
                    if ( url[index] == ':' )
                    {
                        index+=2;
                        state = 1;
                        break;
                    }
                    this->protocol_ += url[index];
                }
                break;
                // Read port and domain
            case 1:
                {
                    if ( url[index] == ':' )
                    {
                        state = 2;
                        break;
                    }

                    if ( url[index] == '/' )
                    {
                        state = 3;
                        break;
                    }

                    this->domain_ += url[index];
                }
                break;
                // Read domain
            case 2:
                {
                    if ( url[index] == '/' )
                    {
                        this->port_ = atoi( this->tmp_.c_str() );
                        this->tmp_ = "";
                        state = 3;
                        break;
                    }

                    this->tmp_ += url[index];
                }
                break;
                // Read Path
            case 3:
                {
                    if ( url[index] == '?' )
                    {
                        state = 4;
                        this->tmp_ = "";
                        break;
                    }

                    this->path_ += url[index];
                }
                break;
                // Read parameter name
            case 4:
                {
                    if ( url[index] == '=' )
                    {
                        state = 5;
                        break;
                    }

                    this->tmp_ += url[index];
                }break;
                // Read parameter value
            case 5:
                {
                    if ( url[index] == '&' )
                    {
                        state = 4;

                        this->parameter_[this->tmp_] = parameter_value;
                        this->tmp_ = "";
                        parameter_value = "";

                        break;
                    }

                    parameter_value += url[index];
                }break;
            default:
                break;
        }

        index++;
    }
    while ( index < url.size() );

    if( !this->tmp_.empty() )
        this->parameter_[this->tmp_] = parameter_value;

    if ( this->path_.empty() )
    {
        this->port_ = atoi( this->tmp_.c_str() );
        this->path_ = "/";
    }
}

HTTPRequest::HTTPRequest( std::string url , std::string method )
{
    Url url_parse( url );

    this->domain_ = url_parse.Domain();
    this->url_    = url_parse.PathWithArg();
    this->port_   = url_parse.Port();

    if ( this->port_ == 0 )
    {
        this->port_ = 80;
    }

    this->method_ = method;
}

HTTPRequest::HTTPRequest()
{ 
}

HTTPRequest::~HTTPRequest()
{
}

void HTTPRequest::WriteCallback( WriteCallbackType callback )
{
    this->write_callback_ = callback;
}

void HTTPRequest::Content( uptr<Buffer> content )
{
    if ( content == nullptr )
    {
        this->ContentLength( 0 );
    }
    else
    {
        this->ContentLength( content->Size() );
        this->content_ = move_ptr( content );
    }
}

void HTTPRequest::Content( std::string content )
{
    this->ContentLength( content.size() );
    this->content_ = make_uptr( Buffer , content );
}

uptr<Buffer> HTTPRequest::Content()
{
    if ( this->content_ == nullptr ) return nullptr;

    return make_uptr( Buffer ,
                      this->content_->Data() ,
                      this->content_->Size() );
}

void HTTPRequest::ContentLength( size_t size )
{
    this->content_length_ = size;
    char buf[32]          = { 0 };
    sprintf( buf , "%lld" , size );
    //ltoa( ( long ) size , buf , 10 );
    this->Header( "Content-Length" , std::string( buf ) );
}

size_t HTTPRequest::ContentLength()
{
    return this->content_length_;
}

void HTTPRequest::Header( std::string key , std::string value )
{
    this->header_[key] = value;
}

std::string HTTPRequest::Header( std::string key )
{
    return this->header_[key];
}

std::string HTTPRequest::RequestUrl()
{
    return this->url_;
}

void HTTPRequest::Parse( uptr<Buffer> data )
{
    if ( data == nullptr )
    {
        return;
    }

    char* pdata = data->Data();
    char* ori_data = data->Data();

    do
    {
        switch ( this->parse_state_ )
        {
            case ParseState::kMethod:
                {
                    if ( *pdata == ' ' )
                    { 
                        this->parse_state_ = ParseState::kUrl;
                        break;
                    }

                    this->method_ += *pdata;
                }
                break;
            case ParseState::kUrl:
                {
                    if ( *pdata == ' ' )
                    {

                        this->parse_state_ = ParseState::kProtocol;
                        break;
                    }

                    this->url_ += *pdata;
                }
                break;
            case ParseState::kProtocol:
                {
                    if ( *pdata == '\r' )
                    {
                        ++pdata;
                        this->parse_state_ = ParseState::kHeadKey;
                    }

                }
                break;
            case ParseState::kHeadKey:
                {
                    if ( *pdata == '\r' )
                    {
                        ++pdata;
                        this->parse_state_ = ParseState::kContent;
                        if ( !this->header_["Content-Length"].empty() )
                        {
                            content_recv_len_     = 0;
                            this->content_length_ =
                                atoll( this->header_["Content-Length"].c_str() );
                        }

                        break;
                    }

                    if ( *pdata == ':' )
                    {
                        ++pdata;
                        this->parse_state_ = ParseState::kHeadValue;
                        break;
                    }

                    this->tmp_key_ += *pdata;
                }
                break;
            case ParseState::kHeadValue:
                {
                    if ( *pdata == '\r' )
                    {
                        ++pdata;
                        this->Header( this->tmp_key_ , this->tmp_value_ );

                        this->tmp_key_     = "";
                        this->tmp_value_   = "";
                        this->parse_state_ = ParseState::kHeadKey;
                        break;
                    }

                    this->tmp_value_ += *pdata;
                }
                break;
            case ParseState::kContent:
                {
                    if ( this->content_length_ > MAX_CIRCLE_BUFFER_SIZE )
                    {
                        return;
                    }
                    else if ( this->content_ == nullptr )
                    {
                        this->content_ = make_uptr( MRT::Buffer , this->content_length_ );
                    }

                    this->content_->Push( pdata ,
                                          data->Size() - ( pdata - ori_data ) );

                    content_recv_len_ += data->Size() - ( pdata - ori_data );

                    return;
                }
                break;
            default:
                break;
        }

        ++pdata;
    }
    while ( ( size_t ) ( pdata - ori_data ) < data->Size() );
}

uptr<Buffer> HTTPRequest::BuildHeader()
{
    std::string head = "";
    std::string new_line = "\r\n";

    head += this->method_ + " /" + this->url_ + " HTTP/1.1" + new_line;

    for ( auto kv : this->header_ )
    {
        head += kv.first + ": " + kv.second + new_line;
    }

    head += new_line;

    return make_uptr( Buffer , head );
}

uptr<Buffer> HTTPRequest::BuildBody()
{
    if ( this->write_callback_ != nullptr )
    {
        auto result = this->write_callback_( this );
        return move_ptr( result );
    }

    if ( this->content_ != nullptr )
    {
        return make_uptr( Buffer ,
                          this->content_->Data() ,
                          this->content_->Size() );
    }

    return nullptr;
}

void* HTTPRequest::Data()
{
    return this->data_;
}

void HTTPRequest::Data( void * value )
{
    this->data_ = value;
}

bool HTTPRequest::Finish()
{
    if ( this->content_ == nullptr )
    {
        return false;
    }

    return content_recv_len_ == this->content_length_;
}

sptr<Url> HTTPRequest::Uri()
{
    return make_sptr( Url , this->url_ );
}

//HTTPResponse::HTTPResponse( size_t Status )
//{
//    this->Status( Status );
//}

HTTPResponse::HTTPResponse()
{
}

HTTPResponse::~HTTPResponse()
{
}

void HTTPResponse::ReadCallback( RepReadCallbackType callback )
{
    this->read_callback_ = callback;
}

void * HTTPResponse::Data()
{
    return this->data_;
}

void HTTPResponse::Data( void * value )
{
    this->data_ = value;
}

void HTTPResponse::Header( std::string key , std::string value )
{
    this->header_[key] = value;
}

std::string HTTPResponse::Header( std::string key )
{
    return this->header_[key];
}

void HTTPResponse::Content( uptr<Buffer> content )
{
    if ( content == nullptr ) return;

    this->ContentLength( content->Size() );
    this->content_ = move_ptr( content );
}

void HTTPResponse::Content( std::string content )
{
    this->ContentLength( content.size() );
    this->content_ = make_uptr( Buffer , content );
}

uptr<Buffer> HTTPResponse::Content()
{
    if ( this->content_ == nullptr ) return nullptr;

    uptr<Buffer> result = make_uptr( Buffer ,
                                     this->content_->Data() ,
                                     this->content_->Size() );
    return result;
}

void HTTPResponse::ContentLength( size_t size )
{
    this->content_length_ = size;
    char buf[16]          = { 0 };
    sprintf( buf , "%lld" , size );
    //ltoa( size , buf , 10 );
    this->Header( "Content-Length" , std::string( buf ) );
}

size_t HTTPResponse::ContentLength()
{
    return this->content_length_;
}

void HTTPResponse::Status( size_t code )
{
    this->status_ = code;

    switch ( this->status_ )
    {
        case 200:
            {
                this->status_str_ = "OK";
            }
            break;
        case 404:
            {
                this->status_str_ = "Not Found";
            }
            break;
        default:
            {
                this->status_str_ = "Unknown";
            }
            break;
    }
}

size_t HTTPResponse::Status()
{
    return this->status_;
}

uptr<Buffer> HTTPResponse::BuildHeader()
{
    std::string head        = "";
    std::string new_line    = "\r\n";
    char tmp[16]            = { 0 };

    sprintf( tmp , "%lld" , this->status_ );
    //ltoa( this->status_ , tmp , 10 );
    std::string status_code_str ( tmp );

    head += "HTTP/1.1 " + status_code_str + " " + this->status_str_ + new_line;

    for ( auto kv : this->header_ )
    {
        head += kv.first + ": " + kv.second + new_line;
    }

    head += new_line;

    return make_uptr( Buffer , head );
}

uptr<Buffer> HTTPResponse::BuildBody()
{
    if ( this->content_ != nullptr )
    {
        return make_uptr( Buffer ,
                          this->content_->Data() ,
                          this->content_->Size() );
    }

    return nullptr;
}

void HTTPResponse::Parse( uptr<Buffer> data )
{
    if ( data == nullptr )
    {
        return;
    }

    char* pdata = data->Data();
    char* ori_data = data->Data();

    do
    {
        switch ( this->parse_state_ )
        {
            case ParseState::kProtocol:
                {
                    if ( *pdata == ' ' )
                    {
                        this->parse_state_ = ParseState::kStatusCode;
                        break;
                    }

                    this->protocol_ += *pdata;
                }
                break;
            case ParseState::kStatusCode:
                {
                    if ( *pdata == ' ' )
                    {
                        char buf[16]        = { 0 };
                        this->status_       = atoll( this->status_str_.c_str() );
                        this->status_str_   = "";
                        this->parse_state_  = ParseState::kStatus;
                        break;
                    }

                    this->status_str_ += *pdata;
                }
                break;
            case ParseState::kStatus:
                {
                    if ( *pdata == '\r' )
                    {
                        ++pdata;
                        this->parse_state_ = ParseState::kHeadKey;
                    }
                    this->status_str_ += *pdata;
                }
                break;
            case ParseState::kHeadKey:
                {
                    if ( *pdata == '\r' )
                    {
                        ++pdata;
                        this->parse_state_ = ParseState::kContent;
                        break;
                    }

                    if ( *pdata == ':' )
                    {
                        ++pdata;
                        this->parse_state_ = ParseState::kHeadValue;
                        break;
                    }

                    this->tmp_key_ += *pdata;
                }
                break;
            case ParseState::kHeadValue:
                {
                    if ( *pdata == '\r' )
                    {
                        ++pdata;
                        this->Header( this->tmp_key_ , this->tmp_value_ );

                        if ( this->tmp_key_ == "Content-Length" )
                        {
                            this->content_length_ = atoll( this->tmp_value_.c_str() );
                            this->content_ = make_uptr( Buffer , this->content_length_ );
                            this->content_->Zero();
                            this->content_recv_len_ = 0;
                        }

                        this->tmp_key_     = "";
                        this->tmp_value_   = "";
                        this->parse_state_ = ParseState::kHeadKey;
                        break;
                    }

                    this->tmp_value_ += *pdata;
                }
                break;
            case ParseState::kContent:
                {
                    if ( this->read_callback_ != nullptr )
                    {
                        size_t delta_size = data->Size() - ( pdata - ori_data );
                        this->read_callback_( this ,
                                              make_uptr( Buffer ,
                                              pdata ,
                                              delta_size )
                                              );
                    }
                    else
                    {

                        if ( this->content_ == nullptr )
                        {
                            this->content_ = make_uptr( MRT::Buffer , 1024 * 1024 );
                        }

                        if ( this->content_length_ > MAX_CIRCLE_BUFFER_SIZE )
                        {
                            return;
                        }

                        auto size = scast<int>( pdata - ori_data );

                        this->content_->Push( pdata ,
                                              data->Size() - size );

                        this->content_recv_len_ +=  data->Size() - size;
                    }
                    return;
                }
                break;
            default:
                break;
        }

        ++pdata;
    }
    while ( ( size_t ) ( pdata - ori_data ) < data->Size() );
}
bool HTTPResponse::Finish()
{
    if ( this->content_ == nullptr &&
         this->content_length_ == 0 )
    {
        return true;
    }

    if ( this->content_ == nullptr &&
         this->content_length_ > 0 )
    {
        return false;
    }

    return this->content_recv_len_ == this->content_length_;
}

NS_MARATON_END

