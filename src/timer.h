// Note: Based on: https://github.com/raysan5/raylib/wiki/Frequently-Asked-Questions#how-do-i-make-a-timer

#ifndef TIMER_H
#define TIMER_H

typedef struct {
    double start_time;
    double life_time;
} Timer;

void timer_start(Timer* timer, double life_time);
int timer_done(const Timer timer);
double timer_get_elapsed(const Timer timer);

#endif