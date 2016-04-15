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

#ifndef NETWORK_DATA_BUILDER_H_
#define NETWORK_DATA_BUILDER_H_

#include <memory.h>
#include <Macro.h>
#include "Buffer.h"

NS_MARATON_BEGIN

// @Description : Circle buffer class
class CircleBuffer
{
public:

    // Default constructure
    CircleBuffer ( );

    // Constructure with buffer size
    CircleBuffer ( size_t size );

    // Default destructure
    ~CircleBuffer( );

    // Push data to the tail of the buffer
    // @buf : Buffer to input
    virtual bool Push           ( Buffer & buf );

    // Push data to the tail of the buffer
    // @data : data pointer
    // @len  : size of the data
    virtual bool Push           ( const char* data , size_t len );

    // Push data to the tail of the buffer
    // @buf : Buffer to input
    virtual bool Push           ( uptr<Buffer> buf );

    // Empty the circlebuffer
    void         Clear          ();

    // Pop data from the circlebuffer, 
    // return nullptr if there is not enough data to pop
    // return uptr<Buffer> if success
    // @len : size to pop
    virtual uptr<Buffer> Pop    ( size_t len );

    // Get the length of the circlebuffer
    size_t Length               () { return this->buffer_length_; };
    
    // Get the used length of the circlebuffer
    size_t UsedLength           () { return this->used_len_; };

    // Get the available length of the circlebuffer
    size_t AvailableLength      () { return this->available_len_; };

     
private:

    CircleBuffer                ( CircleBuffer & ) = delete;
    CircleBuffer                ( CircleBuffer && ) = delete;
    CircleBuffer operator=      ( CircleBuffer ) = delete;

    size_t tail_                = 0;
    size_t head_                = 0;
    size_t used_len_            = 0;
    size_t available_len_       = 0;

    const int buffer_length_    = MAX_CIRCLE_BUFFER_SIZE;

    char* circle_buffer_        = nullptr;
};

NS_MARATON_END

#endif // !NETWORK_DATA_BUILDER_H_