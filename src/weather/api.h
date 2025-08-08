#ifndef WEATHER_API_H
#define WEATHER_API_H

#include "forecasts.h"
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

int weather_get_forecast(WeatherAPI* w_api, WeatherForecast* w_fc);

#endif