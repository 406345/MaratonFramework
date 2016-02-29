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
#ifndef UUID_HPP_
#define UUID_HPP_

#include <random>
#include <string>

#include "Macro.h"

NS_MARATON_BEGIN

class UUID
{
public:

    static std::string Create() 
    {
        std::string ret = "";
        //char buf[kUUIDLength];
        const char *c = "yhgenomics";
        char num_buf[8] = { 0 };

        for (int n = 0; n < 16; ++n )
        {
            int seed = rand() % 255;

#ifdef _WIN32
            sprintf_s( num_buf , "%02X" , seed );
#else
            sprintf( num_buf , "%02X" , seed );
#endif
            //itoa( seed , num_buf , 16 );
            ret += num_buf;
        } 

        return ret;
    } 

private:

    const static int kUUIDLength = 37;

    UUID() {};
    ~UUID() {};
};

NS_MARATON_END

#endif // !UUID_HPP_