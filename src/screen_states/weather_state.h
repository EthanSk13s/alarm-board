#ifndef WEATHER_STATE_H
#define WEATHER_STATE_H

#include "screen_state.h"
#include "../weather/api.h"

typedef struct
{
    Units unit;
    float latitude;
    float longitude;
    char key[WEATHER_API_KEY_SIZE];
} WeatherStateConfig;

void transition_to_wthr_state(ScreenStatePtr state);
void wthr_state_init();

void* wthr_state_update_thread(void* config_data);
void wthr_state_kill_thread();

#endif