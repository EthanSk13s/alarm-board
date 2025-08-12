#include <stdlib.h>

#include "../screen_states/clock_state.h"
#include "../screen_states/alarm_state.h"
#include "../screen_states/weather_state.h"

#include "screen_man.h"

ScreenManager* init_screen_man()
{
    ScreenManager* instance = malloc(sizeof *instance);

    if (instance != NULL)
    {
        transition_to_clock(&instance->state);
    }

    return instance;
}

void screen_man_free()
{
    clean_up_alarm_state();
    clean_up_clock_state();
    clean_up_wthr_state();
}