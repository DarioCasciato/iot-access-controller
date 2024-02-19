// =============================================================================
// Utils | Timer
// =============================================================================

#include "Timer.h"

void Timer::start()
{
    _startTicks = millis();
}

void Timer::stop()
{
    _startTicks = 0;
}

bool Timer::elapsed(uint32_t ms)
{
    if(_startTicks == 0) return 0;

    _delta = millis() - _startTicks;

    return _delta >= ms;
}

uint32_t Timer::remaining(uint32_t ms)
{
    if(_startTicks == 0) return 0;

    _delta = millis() - _startTicks;

    return ms - _delta;
}

uint32_t Timer::elapsedStart()
{
    if(_startTicks == 0) return 0;

    return millis() - _startTicks;
}
