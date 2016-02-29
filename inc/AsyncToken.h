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

#ifndef ASYNC_TOKEN_H_
#define ASYNC_TOKEN_H_

#include <functional>

#include "Macro.h"
#include "uv.h"

NS_MARATON_BEGIN

template<class T>
class AsyncToken
{
public:

    typedef std::function<void( AsyncToken* , uptr<T> )> CallbackType;

    static void Create( CallbackType callback , uptr<T> data )
    {
        AsyncToken* token = new AsyncToken( callback );
        token->Send( move_ptr( data ) );
    }

    AsyncToken( CallbackType callback )
    {
        this->callback_     = callback;
        this->async_.data   = this;
        uv_async_init( uv_default_loop() ,
                       &this->async_ ,
                       AsyncToken::uv_async_callback );
    }

    ~AsyncToken()
    {
        //uv_close( ( uv_handle_t* ) &this->async_ , NULL );
    }

    void Send( uptr<T> data )
    {
        this->data_ = move_ptr( data );
        uv_async_send( &this->async_ );
    }

private:

    static void uv_async_callback( uv_async_t* handle )
    {
        AsyncToken* token = scast<AsyncToken*>( handle->data );

        if ( token->callback_ != nullptr )
        {
            token->callback_( token , move_ptr( token->data_ ) );
        }

        SAFE_DELETE( token );
    }

    uptr<T>         data_        = nullptr;
    uv_async_t      async_       = { 0 };
    CallbackType    callback_    = nullptr;
};


NS_MARATON_END

#endif // !ASYNC_TOKEN_H_
