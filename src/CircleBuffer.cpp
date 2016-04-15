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

#include "CircleBuffer.h"

NS_MARATON_BEGIN

CircleBuffer::CircleBuffer()
{
    this->circle_buffer_ = new char[this->buffer_length_];

    this->available_len_ = this->buffer_length_;
    this->used_len_ = 0;
}

CircleBuffer::CircleBuffer( size_t size )
{
    this->circle_buffer_ = new char[size];

    this->available_len_ = size;
    this->used_len_ = 0;
}

CircleBuffer::~CircleBuffer()
{
    if ( this->circle_buffer_ != nullptr )
    {
        delete this->circle_buffer_;
        this->circle_buffer_ = nullptr;
    }
}

bool CircleBuffer::Push( Buffer & buf )
{
    if ( AvailableLength() < buf.Size() )
    {
        throw "[CircleBuffer.cpp] not enough buffer";
    }

    if ( ( tail_ + buf.Size() ) > this->buffer_length_ )
    {
        char* ptail = this->circle_buffer_ + this->tail_;
        size_t delta_len = this->buffer_length_ - tail_;
        memcpy( ptail, buf.Data(), delta_len );

        memcpy( this->circle_buffer_, 
                buf.Data() + delta_len, 
                buf.Size()  - delta_len );
    }
    else
    {
        char* ptail = this->circle_buffer_ + this->tail_;

        memcpy( ptail, buf.Data() , buf.Size() );
    }

    this->tail_             = ( this->tail_ + buf.Size() ) % this->buffer_length_;
    this->used_len_         += buf.Size();
    this->available_len_    = this->buffer_length_ - this->used_len_;

    return true;
}

bool CircleBuffer::Push( const char * data , size_t len )
{
    if ( AvailableLength() < len )
    {
        throw "[CircleBuffer.cpp] not enough buffer";
        return false;
    }

    if ( ( tail_ + len ) > this->buffer_length_ )
    {
        char* ptail = this->circle_buffer_ + this->tail_;
        size_t delta_len = this->buffer_length_ - tail_;
        memcpy( ptail, data, delta_len );

        memcpy( this->circle_buffer_, 
                data + delta_len, 
                len  - delta_len );
    }
    else
    {
        char* ptail = this->circle_buffer_ + this->tail_;

        memcpy( ptail, data , len );
    }

    this->tail_             = ( this->tail_ + len ) % this->buffer_length_;
    this->used_len_         += len;
    this->available_len_    = this->buffer_length_ - this->used_len_;

    return true;
}

bool CircleBuffer::Push( uptr<Buffer> buf )
{

    if ( AvailableLength() < buf->Size() )
    {
        throw "[CircleBuffer.cpp] not enough buffer";
        return false;
    }

    if ( ( tail_ + buf->Size() ) > this->buffer_length_ )
    {
        char* ptail = this->circle_buffer_ + this->tail_;
        size_t delta_len = this->buffer_length_ - tail_;
        memcpy( ptail, buf->Data(), delta_len );

        memcpy( this->circle_buffer_, 
                buf->Data() + delta_len, 
                buf->Size()  - delta_len );
    }
    else
    {
        char* ptail = this->circle_buffer_ + this->tail_;

        memcpy( ptail, buf->Data() , buf->Size() );
    }

    this->tail_             = ( this->tail_ + buf->Size() ) % this->buffer_length_;
    this->used_len_         += buf->Size();
    this->available_len_    = this->buffer_length_ - this->used_len_;

    return true;
}

void CircleBuffer::Clear()
{
    this->used_len_ = 0;
    this->available_len_ = this->buffer_length_;

    this->tail_ = this->head_ = 0;
}

uptr<Buffer> CircleBuffer::Pop( size_t len )
{
    if ( len > this->UsedLength() )
    {
        return nullptr;
    }

    uptr<Buffer> result = make_uptr( Buffer , len );
    size_t result_pos   = 0;
    char* phead         = this->circle_buffer_ + this->head_;

    // there are two pices Data
    // one is at end of circle buffer
    // antoher is at front of circle buffer
    if ( ( this->head_ + len ) > this->buffer_length_ )
    {
        size_t end_len  = this->buffer_length_ - this->head_;

        memcpy( result->Data() , 
                phead + result_pos,
                end_len );

        result_pos      += this->buffer_length_ - this->head_;

        memcpy( result->Data() + result_pos, 
                this->circle_buffer_, 
                ( len - end_len ) );
    }
    // circle buffer contains all Data inside
    else
    {
        memcpy( result->Data(), phead + result_pos, len );
    }

    this->head_          = ( this->head_ + len ) % this->buffer_length_;
    this->used_len_     -= len;
    this->available_len_ = this->buffer_length_ - this->used_len_;

    return move_ptr(result);
}

NS_MARATON_END