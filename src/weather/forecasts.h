#ifndef FORECASTS_H
#define FORECASTS_H

#include <time.h>
#include <cJSON/cJSON.h>

#define FORECAST_MAX_WEATHER_SIZE  2
#define FORECAST_MAX_DAILY_SIZE    5
#define FORECAST_MAX_SUMMARY_SIZE  124

#define FORECAST_MAX_ICON_LENGTH 4

#define FORECAST_MAX_WEATHER_NAME_LENGTH 12

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
    char icon[FORECAST_MAX_ICON_LENGTH];
    char main[FORECAST_MAX_WEATHER_NAME_LENGTH];
} Weather;

typedef struct
{
    float temp;
    float feels_like;
    Weather weather[FORECAST_MAX_WEATHER_SIZE];
    WindInfo wind;
} CurrentForecast;

typedef struct
{
    float temp_max;
    float temp_min;
    char summary[FORECAST_MAX_SUMMARY_SIZE];
    Weather weather[FORECAST_MAX_WEATHER_SIZE];
    DayTimestamps day;
    WindInfo wind;
} DailyForecast;

typedef struct
{
    time_t tz_offset;
    CurrentForecast current;
    DailyForecast daily[FORECAST_MAX_DAILY_SIZE];
} WeatherForecast;

int forecast_parse_full(WeatherForecast* weather_fc, const cJSON* json);
int forecast_parse_dailies(DailyForecast* daily_fc, const cJSON* daily_json);
int forecast_parse_current(CurrentForecast* current_fc, const cJSON* curr_json);

#endif