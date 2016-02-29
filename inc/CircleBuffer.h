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

class CircleBuffer
{
public:

    CircleBuffer ( );

    ~CircleBuffer( );

    virtual bool Push           ( Buffer & buf );
    virtual bool Push           ( const char* data , size_t len );
    virtual bool Push           ( uptr<Buffer> buf );
    void         Clear          ();

    virtual uptr<Buffer> Pop    ( size_t len );

    size_t Length               () { return this->buffer_length_; };
    size_t UsedLength           () { return this->used_len_; };
    size_t AvailableLength      () { return this->available_len_; };

    size_t tail_ = 0;
    size_t head_ = 0;

private:

    CircleBuffer                ( CircleBuffer & ) = delete;
    CircleBuffer                ( CircleBuffer && ) = delete;
    CircleBuffer operator=      ( CircleBuffer ) = delete;

    size_t used_len_            = 0;
    size_t available_len_       = 0;

    const int buffer_length_    = MAX_CIRCLE_BUFFER_SIZE;

    char* circle_buffer_        = nullptr;
};

NS_MARATON_END

#endif // !NETWORK_DATA_BUILDER_H_