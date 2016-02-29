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

#include "Buffer.h"

#ifndef _WIN32
#include <string.h>
#endif

NS_MARATON_BEGIN

Buffer::Buffer( )
{
    this->data_  = nullptr;
    this->pdata_ = this->data_;
    this->size_  = 0;
}

Buffer::Buffer( size_t size )
{
    SAFE_DELETE( this->data_ );
    this->size_ = size;

    if ( this->size_ <= 0 )
    {
        this->data_ = this->pdata_ = nullptr;
        this->size_ = 0;
        return;
    }

    this->data_  = new char[size] { 0 };
    this->pdata_ = this->data_;
    this->size_  = size;
}

Buffer::Buffer( std::string string )
{
    this->Data( string.c_str( ) , string.size( ) );
}

Buffer::Buffer( const char * data , size_t size )
{
    this->Data( data , size );
}

Buffer & Buffer::operator=( Buffer & buffer )
{
    this->Data( buffer.data_ , buffer.size_ );
    return *this;
}

Buffer & Buffer::operator=( Buffer && buffer )
{
    this->data_  = buffer.data_;
    this->size_  = buffer.size_;
    this->pdata_ = this->data_;

    buffer.data_ = nullptr;
    buffer.size_ = 0;

    return *this;
}

Buffer & Buffer::operator+=( const Buffer & buffer )
{
    size_t total_len   = buffer.size_ + this->size_;
    char* tmp          = new char[total_len]
    {
        0
    };
    memcpy( tmp , this->data_ , this->size_ );
    SAFE_DELETE( this->data_ );
    memcpy( tmp + this->size_ , buffer.data_ , buffer.size_ );
    this->data_ = tmp;
    this->size_ = total_len;
    return *this;
}

char Buffer::operator[]( const size_t index )
{
    if ( this->data_ == nullptr )
    {
        throw "Buffer.data is nullptr";
    }

    if ( this->size_ <= index )
    {
        throw "Buffer out of size!!!";
    }

    return *( this->data_ + index );
}

Buffer::~Buffer( )
{
    SAFE_DELETE( this->data_ );
}

Buffer::Buffer( Buffer & buffer )
{
    this->Data( buffer.data_ , buffer.size_ );
}

Buffer::Buffer( Buffer && buffer )
{
    this->data_  = buffer.data_;
    this->size_  = buffer.size_;
    this->pdata_ = this->data_;

    buffer.data_ = nullptr;
    buffer.size_ = 0;
}

char * Buffer::Data( )
{
    return this->data_;
}

void Buffer::Data( const char * value , size_t size )
{
    SAFE_DELETE( this->data_ );
    this->size_ = size;

    if ( this->size_ <= 0 )
    {
        this->data_ = this->pdata_ = nullptr;
        this->size_ = 0;
        return;
    }

    if ( value == nullptr )
    {
        this->data_ = this->pdata_ = nullptr;
        return;
    }


    this->data_     = new char[size] { 0 };
    this->pdata_    = this->data_;

    memcpy( this->data_ , value , size );
}

void Buffer::Push( const char * data , size_t len )
{
    if ( this->data_ == nullptr )
        return;

    size_t delta = this->size_ - ( size_t ) ( this->pdata_ - this->data_ );

    if ( delta > len )
    {
        memcpy( this->pdata_ , data , len );
    }
    else
    {
        memcpy( this->pdata_ , data , delta );
    }

    this->pdata_+=delta;
}

void Buffer::Zero( )
{
    memset( this->data_ , (int)this->size_ , 0 );
}

void Buffer::ClearUp( )
{
    SAFE_DELETE( this->data_ );
    this->size_ = 0;
}

NS_MARATON_END