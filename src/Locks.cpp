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

#include "Locks.h"

NS_MARATON_BEGIN

Mutex::Mutex( )
{
    uv_mutex_init( &this->mutex_ );
}

Mutex::~Mutex( )
{
    uv_mutex_destroy( &this->mutex_ );
}

void Mutex::Wait( )
{
    uv_mutex_lock( &this->mutex_ );
}

void Mutex::Release( )
{
    uv_mutex_unlock( &this->mutex_ );
}
 
Semaphore::Semaphore( )
{
    uv_sem_init( &this->sem_ , 0 );
}

Semaphore::Semaphore( int initNum )
{
     uv_sem_init( &this->sem_ , initNum );
}

Semaphore::~Semaphore( )
{
    uv_sem_destroy( &this->sem_ );
}

void Semaphore::Wait( )
{
    uv_sem_wait( &this->sem_ );
}

void Semaphore::Release( )
{
    uv_sem_post( &this->sem_ );
}

Signal::Signal()
{
    uv_rwlock_init( &this->locker_ );
}

Signal::~Signal()
{
    uv_rwlock_destroy( &this->locker_ );
}

void Signal::Wait()
{
    uv_rwlock_wrlock( &this->locker_ );
}

void Signal::Release()
{
    uv_rwlock_wrunlock( &this->locker_ );
}

NS_MARATON_END
