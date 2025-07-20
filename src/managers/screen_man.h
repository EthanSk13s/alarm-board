#ifndef SCREEN_MAN_H
#define SCREEN_MAN_H

#include "../screen_states/screen_state.h"

typedef struct
{
    struct ScreenState state;
} ScreenManager;

ScreenManager* init_screen_man();
void clock_screen();
void alarm_screen();

void screen_man_free();

#endif