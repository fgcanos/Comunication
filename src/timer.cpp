#include "timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}


void Timer::set(unsigned long time_elapsed)
{
    timer = millis() + time_elapsed;
}

bool Timer::elapsed()
{
    bool ret = false;

    if( millis() >= timer )
    {
        ret = true;
    }

    return(ret);
}

void Timer::set_us(unsigned long time_elapsed)
{
    timer_us = micros() + time_elapsed;
}

bool Timer::elapsed_us()
{
    bool ret = false;

    if( micros() >= timer_us )
    {
        ret = true;
    }

    return(ret);
}