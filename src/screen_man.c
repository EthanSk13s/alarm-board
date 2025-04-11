#include <stdio.h>
#include <stdlib.h>

#include "screen_states/clock_state.h"
#include "time.h"
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