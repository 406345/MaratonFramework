#include "Thread.h"

NS::Thread::Thread( thread_callback_t cb , void * data )
{
    this->callback_ = cb;
    this->data_     = data;

    uv_thread_create( &this->thread_ , on_thread_callback , this );
}

NS::Thread::~Thread()
{

}

void NS::Thread::Wait()
{
    uv_thread_join( &this->thread_ );
}
 
void NS::Thread::on_thread_callback( void * argv )
{
    Thread* thr = ( Thread* ) argv;
    
    if ( thr->callback_ != nullptr )
    {
        thr->callback_( thr->data_ );
    }
}
