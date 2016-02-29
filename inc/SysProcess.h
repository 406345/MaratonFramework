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

#ifndef PRCOESS_H_
#define PRCOESS_H_

#include <stdio.h>
#include <functional>
#include <string>

#include "uv.h"
#include "Macro.h"
#ifdef _WIN32
#include <windows.h>
#else

#endif

NS_MARATON_BEGIN

class SysProcess
{
public:

    typedef std::function<void( SysProcess* , size_t )> PrceossCallbackType;
    
    static SysProcess* Create( std::string  file , std::string  args , std::string  directry , PrceossCallbackType on_finish );
    static SysProcess* Create( std::string  file , std::string  args , PrceossCallbackType on_finish );
    static SysProcess* Create( std::string  file , PrceossCallbackType on_finish );
    static void Desctroy( SysProcess** process );

    
    void* Data() { return this->data_; };
    void  Data( void* pdata ) { this->data_ = pdata; };

    std::string StdOut() { return this->std_out_; };

    void Start();
    void Kill();

private:

    static void uv_process_exit_callback( uv_process_t* , int64_t exit_status , int term_signal );
    static void uv_process_close_callback( uv_handle_t* handle );
    static void uv_process_alloc_buffer( uv_handle_t *handle , size_t suggested_size , uv_buf_t *buf );
    static void uv_prcoess_read_stream( uv_stream_t *stream , ssize_t nread , const uv_buf_t *buf );
   
    SysProcess();
    SysProcess( std::string  file , std::string  args , std::string  directry , PrceossCallbackType on_finish );
    SysProcess( std::string  file , std::string  args , PrceossCallbackType on_finish );
    SysProcess( std::string  file , PrceossCallbackType on_finish );
    ~SysProcess();

    size_t WaitForExit();

#ifdef _WIN32
    STARTUPINFO si_;
    PROCESS_INFORMATION pi_;
#else

    FILE* p_stream = NULL;

#endif

    PrceossCallbackType callback_;

    void Invoke();

    const int   kStrLength = 1024;

    char*       file_ = NULL;
    char*       args_ = NULL;;
    char*       directory_ = NULL;

    int         result_ = 0;

    uv_sem_t    sem;

    void*       data_ = nullptr;

    uv_process_t child_req;
    uv_process_options_t options = { 0 };
    uv_pipe_t   pipe_;

    std::string std_out_;
};

NS_MARATON_END

#endif // !PROCESS_H_