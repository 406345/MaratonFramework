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

#include "Operator.h"

NS_MARATON_BEGIN

Operator::Operator( std::string address , int port )
{
    this->address_ = address;
    this->port_    = port;
}

Operator::~Operator( )
{
}

std::string Operator::IP( )
{
    return this->ip_;
}

std::string Operator::Address( )
{
    return this->address_;
}

int Operator::Port( )
{
    return this->port_;
}

NS_MARATON_END
