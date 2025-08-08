#ifndef WEATHER_API_H
#define WEATHER_API_H

#include "../networking/net.h"

typedef struct WeatherConfig WeatherConfig;

typedef enum
{
    KELVIN,
    IMPERIAL,
    METRIC
} Units;

typedef struct
{
    NetworkHandler net_handler;
    WeatherConfig* _config;
} WeatherAPI;

int weather_init_api(WeatherAPI* w_api, char* key, Units unit, float latitude, float longitude);
void weather_free_api(WeatherAPI* w_api);

#endif