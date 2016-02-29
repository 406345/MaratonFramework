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

#include "SysProcess.h"
#include <thread>
#include <string.h>
#include <memory>
#include <thread>

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

NS_MARATON_BEGIN


SysProcess::~SysProcess()
{

    if ( this->file_ != NULL )
    {
        delete this->file_;
        this->file_ = NULL;
    }

    if ( this->args_ != NULL )
    {
        delete this->args_;
        this->args_ = NULL;
    }

    if ( this->directory_ != NULL )
    {
        delete this->directory_;
        this->directory_ = NULL;
    }
    
}
 
void SysProcess::Desctroy( SysProcess ** process )
{
    if ( process != nullptr && *process != nullptr )
    {
        delete *process;
        *process = nullptr;
    }
}

SysProcess * SysProcess::Create( std::string file , std::string args , 
                                 std::string directry ,
                                 PrceossCallbackType on_finish )
{
    auto ret = new SysProcess( file , args , directry , on_finish );
    return ret;
}

SysProcess * SysProcess::Create( std::string file , std::string args ,
                                 PrceossCallbackType on_finish )
{
    auto ret = new SysProcess( file , args ,  on_finish );
    return ret;
}

SysProcess * SysProcess::Create( std::string file ,
                                 PrceossCallbackType on_finish )
{
    auto ret = new SysProcess( file , on_finish );
    return ret;
}  

void SysProcess::uv_process_exit_callback( uv_process_t * process , 
                                           int64_t exit_status , 
                                           int term_signal )
{
    SysProcess* instance = static_cast< SysProcess* >( process->data );
    uv_close( ( uv_handle_t* )process , SysProcess::uv_process_close_callback );
    instance->callback_( instance , exit_status ); 
}

void SysProcess::uv_process_close_callback( uv_handle_t * handle )
{
    SysProcess* instance = static_cast< SysProcess* >( handle->data );
    SAFE_DELETE( instance );
}

void SysProcess::uv_process_alloc_buffer( uv_handle_t * handle , 
                                          size_t suggested_size ,
                                          uv_buf_t * buf )
{
    *buf = uv_buf_init( ( char* )malloc( suggested_size ) , (unsigned int) suggested_size );
}

void SysProcess::uv_prcoess_read_stream( uv_stream_t * stream , 
                                         ssize_t nread , 
                                         const uv_buf_t * buf )
{
    SysProcess* inst = static_cast< SysProcess* > ( stream->data );

    if ( nread < 0 )
    {
        if ( nread == UV_EOF )
        {
            // end of file
            uv_close( ( uv_handle_t * )&inst->pipe_ , NULL );
        }

        return;
    } 

    inst->std_out_ = std::string( buf->base , nread );

    if ( buf->base )
        delete buf->base;
}

SysProcess::SysProcess()
{
    //uv_sem_init( &this->sem , 0 );
}

SysProcess::SysProcess( std::string  file, std::string args, 
                        std::string  directry,
                        PrceossCallbackType on_finish )
    : SysProcess()
{
    std::string newArgs = args;

    this->file_ = new char[this->kStrLength];
    this->args_ = new char[this->kStrLength];
    this->directory_ = new char[this->kStrLength];
    memset( this->file_, 0, this->kStrLength );
    memset( this->args_, 0, this->kStrLength );
    memset( this->directory_, 0, this->kStrLength );

    memcpy( this->file_, file.c_str() , file.length() );
    memcpy( this->args_, newArgs.c_str(), newArgs.length() );
    memcpy( this->directory_, directry.c_str(), directry.length() );
    this->callback_ = on_finish;
}

SysProcess::SysProcess( std::string  file, std::string args, 
                        PrceossCallbackType on_finish )
    : SysProcess()
{
    std::string newArgs =  args;

    this->file_ = new char[this->kStrLength];
    this->args_ = new char[this->kStrLength]; 
    memset( this->file_, 0, this->kStrLength );
    memset( this->args_, 0, this->kStrLength ); 

    memcpy( this->file_, file.c_str(), file.length() );
    memcpy( this->args_, newArgs.c_str(), newArgs.length() );

    this->callback_ = on_finish; 
}

SysProcess::SysProcess( std::string  file, PrceossCallbackType on_finish )
    : SysProcess()
{
    this->file_ = new char[this->kStrLength]; 
    memset( this->file_, 0, this->kStrLength );

    memcpy( this->file_, file.c_str(), file.length() ); 
    this->callback_ = on_finish;  
}

void SysProcess::Invoke()
{ 
    int r;
    char** args = NULL;

    //if ( this->directory_ == nullptr )
    //{
    //    char Path[512] = { 0 };
    //    getcwd( Path , 512 );
    //    int path_len = strlen( Path );
    //    this->directory_ = new char[path_len + 1];
    //    memset( this->directory_ , 0 , path_len + 1 );
    //    memcpy( this->directory_ , Path , strlen( Path ) );
    //}
    printf( "File:%s\r\n" , this->file_ );
    if ( args_ == nullptr )
    {
        //char Path[512] = { 0 };
        //getcwd( Path , 512 );
        //args = new char*[2];
        //args[0] = ' ';
        //args[1] = NULL;
        //args[0] = new char[strlen( Path ) + 1];
        //memset( args[0] , 0 , strlen( Path ) + 1 );
        //memcpy( args[0] , Path , strlen( Path ) );
        //args[1] = NULL;
    } 
    else
    {
        args = new char*[64];
        for ( size_t i = 0; i < 64; i++ )
        {
            args[i] = new char[512];
            memset( args[i] , 0 , 512 );
            args[i][0] = ' ';
        }
        auto raw_args = this->args_;
        size_t len = strlen( this->args_ );

        int start_pos = 0; 

#ifdef _WIN32
        int row = 0;
#else
        int row = 0;
#endif
        int col = 0;
        bool has_dot = false;
        for ( int e = start_pos; e < len; e++ )
        {
            if ( raw_args[e] == '\'')
            {
                if ( has_dot )
                    has_dot = false;
                else
                    has_dot = true;
            }

            if ( raw_args[e] == ' ' && !has_dot)
            {
                printf( "Args:%s\r\n", args[row] );
                col = 0;
                row++; 
                args[row][col] = ' ';
                col = 1;
            }
            else 
            {
                args[row][col] = raw_args[e];
                col++;
            }
        }
         
        printf( "Args:%s\r\n" , args[row] );
        args[row+1] = NULL;
    }
    auto loop = uv_default_loop();
    this->pipe_ = { 0 };

    r = uv_pipe_init( loop , &this->pipe_ , 0 );
     
    this->pipe_.data = this;
    
    this->options.exit_cb = SysProcess::uv_process_exit_callback;
    this->options.file = this->file_;
    this->options.args = args;
    this->options.cwd = this->directory_;
    this->child_req.data = this;

    this->options.stdio_count = 3;
    uv_stdio_container_t child_stdio[3];

    // ( uv_stdio_flags )( UV_CREATE_PIPE | UV_READABLE_PIPE );
    child_stdio[0].flags = UV_IGNORE;

    child_stdio[1].flags = ( uv_stdio_flags )( UV_CREATE_PIPE | UV_WRITABLE_PIPE );
    child_stdio[1].data.stream = ( uv_stream_t* )&this->pipe_;

    child_stdio[2].flags = UV_IGNORE;

    this->options.stdio = child_stdio;
    
    r  = uv_spawn( loop , &this->child_req , &this->options );
    if ( r != 0 )
    {
        printf( "uv_spawn: %s\r\n" , uv_strerror( r ) );
    }

    r = uv_read_start( ( uv_stream_t* )&this->pipe_ ,
                       SysProcess::uv_process_alloc_buffer , 
                       SysProcess::uv_prcoess_read_stream );
    if ( r != 0 )
    {
        printf( "uv_read_start: %s\r\n" , uv_strerror( r ) );
    }

    delete[] args;
}

size_t SysProcess::WaitForExit()
{ 
   // uv_sem_wait( &this->sem );
    //uv_sem_destroy( &this->sem );
    return this->result_;
}

void SysProcess::Start()
{
    this->Invoke();
}

void SysProcess::Kill()
{

    uv_process_kill( &this->child_req , 1 );

#ifdef _WIN32
    
    //TerminateProcess( this->pi_.hProcess, -1 );

#else

    pclose( p_stream );

#endif // _WIN32   
}


NS_MARATON_END