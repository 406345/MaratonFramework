/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-11
* Description:
* * * * * * * * * * * * * * * */

#ifndef LOOP_EVENT_H_
#define LOOP_EVENT_H_

#include <uv.h>
#include <Macro.h>

NS_MARATON_BEGIN

class LoopEvent
{
public:

    LoopEvent();
    ~LoopEvent();

    uv_loop_t* Event();
    void       Wait();
    void       RunOnce( );

private:

    uv_loop_t loop_;

};

NS_MARATON_END

#endif // !LOOP_EVENT_H_
