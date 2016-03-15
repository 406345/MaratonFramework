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

#ifndef BUFFER_H_
#define BUFFER_H_

#include <string>
#include "Macro.h"

NS_MARATON_BEGIN
// @Description : Storage data and it's length
class Buffer
{
public:

    // Default constructure      
    Buffer();

    ~Buffer();

    // Initial with the data size
    // @size : Buffer size
    Buffer( size_t size );

    // Initial with the string 
    // @string  : String data
    Buffer( std::string string );

    // Initial with the data pointer and it's length
    // @data   : data pointer
    // @size   : data size
    // @note   : the data size must longer than the size
    Buffer( const char* data , size_t size );

    // Initial with the other buffer, copy it's data
    // @buffer : Buffer pointer
    Buffer( Buffer& buffer );
    
    // Initial with the other buffer, move it's data
    // @buffer : Buffer pointer
    Buffer( Buffer&& buffer );
    Buffer& operator =( Buffer& buffer );
    Buffer& operator =( Buffer&& buffer );
    Buffer& operator +=( const Buffer& buffer );

    // Get data at the index
    // @index : position
    char    operator[]( const size_t index );

    // Get data
    char*   Data();

    // Set the data
    // @value : data pointer
    // @size  : size of the data
    void    Data( const char * value , size_t size );

    // Get the buffer size
    size_t  Size() { return this->size_; }

    // Insert data to the tail of the buffer
    // @data : data pointer
    // @len  : size of the data
    void    Push( const char* data , size_t len );

    // Empty the buffer
    void    Zero();

private:

    char*   data_  = nullptr;
    size_t  size_  = 0;
    char*   pdata_ = nullptr;

    void    ClearUp();
};

NS_MARATON_END

#endif // !BUFFER_H_
