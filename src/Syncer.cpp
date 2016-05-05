#include <Syncer.h>

NS::Syncer::Syncer( sptr<LoopEvent> loop , syncer_callback_t cb )
{
    this->callback_ = cb;
    this->loop_     = loop;

    uv_async_init( loop->Event() ,
                   &this->async_ ,
                   async_cb );
    this->async_.data = this;

    uv_async_send( &this->async_ );
}

void NS::Syncer::async_cb( uv_async_t * handle )
{
    Syncer* sync = ( Syncer* ) handle->data;

    if ( sync->callback_ != nullptr )
    {
        sync->callback_();
    }
}
