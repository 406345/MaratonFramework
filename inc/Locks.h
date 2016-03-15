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

#ifndef LOCKS_H_
#define LOCKS_H_

#include "Macro.h"
#include "uv.h"

NS_MARATON_BEGIN

// @Description : provide a mutex lock
class Mutex
{
public:

    // Default constructor
    Mutex( );
    ~Mutex( );

    // Wait for a mutex lock
    void Wait   ();

    // Release a mutex lock
    void Release();

private:

    uv_mutex_t mutex_ = { 0 };
};

// @Description : provide a semaphore lock
class Semaphore
{
public:

    // Default constructor
    Semaphore( );

    // Initial with semaphore max counter 
    Semaphore( int initNum );

    ~Semaphore( );

    // Check if need waiting
    void Wait( );

    // Decrease the counter
    void Release( );

private:

    uv_sem_t sem_ = { 0 };
};

NS_MARATON_END

#endif // !LOCKS_H_
