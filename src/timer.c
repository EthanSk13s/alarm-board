#include <raylib.h>

#include "timer.h"

void timer_start(Timer* timer, double life_time)
{
    timer->start_time = GetTime();
    timer->life_time = life_time;
}

int timer_done(const Timer timer)
{
    return GetTime() - timer.start_time >= timer.life_time;
}

double timer_get_elapsed(const Timer timer)
{
    return GetTime() - timer.start_time;
}