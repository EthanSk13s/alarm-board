#ifndef ALARM_H
#define ALARM_H

#include <raylib.h>

int load_alarm();
int is_alarm_playing();

void play_alarm();
void stop_alarm();
void update_stream();

#endif