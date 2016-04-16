#include <FileSystem.h>
#include <Maraton.h>

NS_MARATON_BEGIN

FileSystem::FileSystem( )
{
    this->loop_event_  = make_sptr( LoopEvent );
}

FileSystem::FileSystem( sptr<LoopEvent> loop )
{
    this->loop_event_ = Maraton::Instance( )->Event( );
}

FileSystem::~FileSystem( )
{

}

void FileSystem::Open( string path , fs_callback_t callback )
{
    this->path_ = path;
    this->mode_ = 0x00007;
    this->flag_ = 0x0002; 

    FileSystemToken* token =  new FileSystemToken( this );
    token->open_callback = callback;
    int hresult = uv_fs_open( this->loop_event_->Event( ) ,
                              token->req_ ,
                              path.c_str( ) ,
                              this->flag_ ,
                              this->mode_,
                              uv_open_callback );

    if ( hresult < 0 )
    { 
        token->open_callback( hresult , this );
        SAFE_DELETE( token );
        return;
    }
}

int FileSystem::Read( size_t size , fs_callback_read_t cb )
{
    auto fsize = this->Tell( );

    if ( fsize < (this->offset_+size) )
    {
        return 0;
    }

    auto hresult = this->Read( this->offset_ ,
                               size ,
                               cb );

    if ( hresult == 0 )
    {
        this->offset_ += size;
    }

    return hresult;
}

int FileSystem::Read( size_t offset , size_t size , fs_callback_read_t cb )
{
    FileSystemToken* token =  new FileSystemToken( this );
    token->read_callback = cb;

    token->buffer_ = new uv_buf_t( );
    token->buffer_->base = new char[size]{0};
    token->buffer_->len  = size;

    int hresult = uv_fs_read( this->loop_event_->Event( ) ,
                              token->req_ ,
                              (int)token->uv_file_ ,
                              token->buffer_ , 1 ,
                              this->offset_ ,
                              uv_read_callback );

    if ( hresult < 0 )
    { 
        token->read_callback( hresult , token->fs_ , nullptr );
        SAFE_DELETE( token );
        return hresult;
    }
    return hresult;
}

int FileSystem::Write( sptr<Buffer> buffer , fs_callback_write_t cb )
{
    auto hresult = this->Write( this->offset_ , buffer , cb );
    
    if ( hresult == 0 )
    {
        this->offset_ += buffer->Size( );
    }

    return hresult;
}

int FileSystem::Write( size_t offset , sptr<Buffer> buffer , fs_callback_write_t cb )
{
    FileSystemToken* token =  new FileSystemToken( this );
    token->write_callback = cb;

    token->buffer_ = new uv_buf_t( );
    token->buffer_->base = new char[buffer->Size()]{0};
    token->buffer_->len  = buffer->Size( );

    memcpy( token->buffer_->base ,
            buffer->Data( ) ,
            buffer->Size( ) );

    int hresult = uv_fs_write( this->loop_event_->Event( ) ,
                               token->req_ ,
                               (int)token->uv_file_ ,
                               token->buffer_ , 1 ,
                               offset ,
                               uv_write_callback );

    if ( hresult < 0 )
    {
        token->write_callback( hresult , token->fs_ );
        SAFE_DELETE( token );
        return hresult;
    }

    return hresult;
}

size_t FileSystem::Tell( )
{
    uv_fs_t req_ = { 0 };
    auto hresult = uv_fs_stat  ( this->loop_event_->Event( ) ,
                                 &req_ ,
                                 this->path_.c_str() , 
                                 NULL );

    if ( hresult < 0 )
        return 0;

    return req_.statbuf.st_size;
}

size_t FileSystem::Seek( long long offset , FS_SEEK mode )
{
    switch ( mode )
    {
        case MRT::FS_BEGIN:
        {
            if ( offset < 0 )
            {
                this->offset_ = 0;
            }
            else
            {
                this->offset_ = offset;
            }
        }
        break;
        case MRT::FS_OFFSET:
        {
            long long tmp = offset + this->offset_;
            if ( tmp < 0 )
            {
                this->offset_ = 0;
            }
            else
            {
                this->offset_ = tmp;
            }
        }
        break;
        case MRT::FS_END:
        {
            auto size = this->Tell( );
            this->offset_ = size - offset;
        }
        break;
        default:
        break;
    }

    return this->offset_;
}

string FileSystem::ErrorStr( int code )
{
    return string( uv_strerror(code) );
}
 
void FileSystem::uv_open_callback( uv_fs_t * req )
{
    FileSystemToken * token = ( FileSystemToken* ) req->data;
    token->fs_->uv_file_    = req->result;
    token->open_callback( req->result , token->fs_ );
    SAFE_DELETE( token );
}

void FileSystem::uv_read_callback( uv_fs_t * req )
{
    FileSystemToken * token = ( FileSystemToken* ) req->data;

    sptr<Buffer> buf = nullptr;

    if ( req->result < 0 )
    {
    }
    else
    {
        token->fs_->offset_ += req->result;
        buf = make_sptr( Buffer , token->buffer_->base , token->buffer_->len );
    }

    token->read_callback( req->result , token->fs_ , buf );

    SAFE_DELETE( token->buffer_ );
    SAFE_DELETE( token );
}

void FileSystem::uv_write_callback( uv_fs_t * req )
{
    FileSystemToken * token = ( FileSystemToken* ) req->data;

    token->write_callback( req->result , token->fs_ );

    SAFE_DELETE( token->buffer_ );
    SAFE_DELETE( token );
}



















NS_MARATON_END
