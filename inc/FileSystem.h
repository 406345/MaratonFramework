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
* Creator       : Shubo Yang
* Date          :
* Modifed       : When      | Who       | What
***********************************************************************************/

#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#include <string>
#include <functional>
#include <uv.h>
#include <Macro.h>
#include <Buffer.h>
#include <LoopEvent.h>

using std::string;
using std::function;

NS_MARATON_BEGIN

class FileSystem;
class FileSystemToken; 

typedef function<void( long long status , FileSystem* fs)> fs_callback_t;
typedef function<void( long long status , FileSystem* fs , sptr<Buffer> buffer )> fs_callback_read_t;
typedef function<void( long long status , FileSystem* fs )> fs_callback_write_t;

enum FS_SEEK
{
    FS_BEGIN = 1,
    FS_OFFSET,
    FS_END
};

class FileSystem
{
public:

    FileSystem ( );
    FileSystem ( sptr<LoopEvent> loop );
    ~FileSystem( );

    // Open a file for reading and write in binary mode
    // @path : file path
    // @cb   : lambda expression
    //         void( long long status , FileSystem* fs
    void Open ( string path , fs_callback_t cb );

    // Read data from the file and move the offset
    // @position : file offset
    // @size     : read size
    // @cb       : callback after data is read 
    //             lambda expression 
    //             void( long long status , FileSystem* fs , sptr<Buffer> buffer )
    int Read ( size_t size , fs_callback_read_t cb);

    // Read data from the file
    // !!!This function will not move the offset!!!
    // @position : file offset
    // @size     : read size
    // @cb       : callback after data is read 
    //             lambda expression 
    //             void( long long status , FileSystem* fs , sptr<Buffer> buffer )
    int Read ( size_t offset , size_t size , fs_callback_read_t cb);

    // Write data to the file and move the offset
    // @position : file offset
    // @buffer   : buffer to write
    // @cb       : callback after data is writen 
    //             lambda expression 
    //             void( long long status , FileSystem* fs 
    int Write( sptr<Buffer> buffer , fs_callback_write_t cb);
    
    // Write data to the file
    // !!!This function will not move the offset!!!
    // @position : file offset
    // @buffer   : buffer to write
    // @cb       : callback after data is writen 
    //             lambda expression 
    //             void( long long status , FileSystem* fs 
    int Write( size_t offset , sptr<Buffer> buffer , fs_callback_write_t cb);

    size_t Tell( );
    size_t Seek( long long offset , FS_SEEK mode );

    string ErrorStr( int code );

private:

    sptr<LoopEvent>    loop_event_    = nullptr;
    long long          uv_file_       = 0;
    size_t             offset_        = 0;
    int                mode_          = 0;
    int                flag_          = 0;
    string             path_          = "";
    
    static void uv_open_callback ( uv_fs_t* req );
    static void uv_read_callback ( uv_fs_t* req );
    static void uv_write_callback( uv_fs_t* req );

    friend class FileSystemToken;

};

class FileSystemToken
{
private:

    FileSystemToken( FileSystem * fs )
    {
        this->req_       = new uv_fs_t( );
        this->fs_        = fs;
        this->req_->data = this;
        this->uv_file_   = fs->uv_file_;
    }
    
    ~FileSystemToken( )
    { 
        uv_fs_req_cleanup( req_ );
        SAFE_DELETE( req_ );
    }

    FileSystem*        fs_              = nullptr;
    uv_fs_t*           req_             = nullptr;
    long long          uv_file_         = 0;
    uv_buf_t*          buffer_          = nullptr;
                                        
    fs_callback_t      open_callback    = nullptr;
    fs_callback_read_t read_callback    = nullptr;
    fs_callback_write_t write_callback  = nullptr;

    friend class FileSystem;
};


NS_MARATON_END

#endif // !FILE_SYSTEM_H_