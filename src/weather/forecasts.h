#ifndef FORECASTS_H
#define FORECASTS_H

#include <time.h>
#include <cJSON/cJSON.h>

typedef struct
{
    int deg;
    float speed;
    float gust;
} WindInfo;

typedef struct
{
    time_t dt;
    time_t sunrise;
    time_t sunset;
} DayTimestamps;

typedef struct
{
    int id;
    char icon[4];
    char main[12];
} Weather;

typedef struct
{
    float temp;
    float feels_like;
    Weather weather[2];
    WindInfo wind;
} CurrentForecast;

typedef struct
{
    float temp_max;
    float temp_min;
    Weather weather[2];
    DayTimestamps day;
    WindInfo wind;
} DailyForecast;

typedef struct
{
    time_t tz_offset;
    CurrentForecast current;
    DailyForecast daily[5];
} WeatherForecast;

int forecast_parse_full(WeatherForecast* weather_fc, const cJSON* json);
int forecast_parse_dailies(DailyForecast* daily_fc, const cJSON* daily_json);
int forecast_parse_current(CurrentForecast* current_fc, const cJSON* curr_json);

#endif