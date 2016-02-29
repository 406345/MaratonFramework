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

#ifndef MANAGER_HPP_
#define MANAGER_HPP_ 

#include <vector>
#include <mutex>
#include "Macro.h"

NS_MARATON_BEGIN

template<class T>
class Manager
{
public:

    virtual void Push( sptr<T> instance )
    {
        this->instance_.push_back( move_ptr( instance ) );
    }

    virtual bool Pop( sptr<T> instance )
    {
        typename std::vector<sptr<T>>::iterator itr = this->instance_.begin( );

        while ( itr != this->instance_.end( ) )
        {
            if ( ( *itr ).get( ) == instance.get( ) )
            {
                itr = this->instance_.erase( itr );
                return true;
            }
            else
            {
                itr++;
            }
        }

        return false;
    }

protected:

    std::vector<sptr<T>> Instances() 
    {
        std::vector<sptr<T>> vret( this->instance_.begin(), this->instance_.end() );

        return vret;
    }

    std::vector<sptr<T>> instance_;

private:

    std::mutex mtx_;
};
 
NS_MARATON_END

#endif //!MANAGER_HPP_ 