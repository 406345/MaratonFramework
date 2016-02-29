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

class Buffer
{
public:

    Buffer();
    ~Buffer();

    Buffer( size_t size );
    Buffer( std::string string );
    Buffer( const char* data , size_t size );
    Buffer( Buffer& buffer );
    Buffer( Buffer&& buffer );
    Buffer& operator =( Buffer& buffer );
    Buffer& operator =( Buffer&& buffer );
    Buffer& operator +=( const Buffer& buffer );
    char    operator[]( const size_t index );

    char*   Data();
    void    Data( const char * value , size_t size );
    size_t  Size() { return this->size_; }
    void    Push( const char* data , size_t len );
    void    Zero();

private:

    char*   data_  = nullptr;
    size_t  size_  = 0;
    char*   pdata_ = nullptr;

    void    ClearUp();
};

NS_MARATON_END

#endif // !BUFFER_H_
