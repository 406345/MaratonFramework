#include <LoopEvent.h>

NS_MARATON_BEGIN

LoopEvent::LoopEvent()
{
    uv_loop_init( &this->loop_ );
}

LoopEvent::~LoopEvent()
{

}

uv_loop_t * LoopEvent::Event()
{
    return &this->loop_;
}

void LoopEvent::Wait()
{
    uv_run( &this->loop_ , UV_RUN_DEFAULT );
}

void LoopEvent::RunOnce( )
{
    uv_run( &this->loop_ , UV_RUN_ONCE );
}

NS_MARATON_END

