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

#ifndef SINGLETON_H_
#define SINGLETON_H_

#include "Macro.h"

NS_MARATON_BEGIN

template<class T>
class Singleton
{
public:
    
    static T* Instance();

protected:

    Singleton();
    ~Singleton();
    static T* instance_ ;
};

template<class T>
T* Singleton<T>::instance_ = nullptr;

template<class T>
Singleton<T>::~Singleton()
{

}

template<class T>
T * Singleton<T>::Instance()
{
    if ( instance_ == nullptr )
    {
        instance_ = new T();
    }

    return instance_;
}

template<class T>
Singleton<T>::Singleton()
{

}

NS_MARATON_END

#endif // !SINGLETON_H_


 