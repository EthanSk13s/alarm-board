#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "forecasts.h"

#define API_KEY_SIZE 33
#define API_URL_TEMPLATE "https://api.openweathermap.org/data/3.0/onecall?lat=%f&lon=%f&exclude=hourly,minutely&appid=%s&units=%s"

#define UNITS_KELVIN   "standard"
#define UNITS_IMPERIAL "imperial"
#define UNITS_METRIC   "metric"

typedef struct
{
    float latitude;
    float longitude;
} GPSCoordinates;

struct WeatherConfig
{
    Units unit;
    char api[API_KEY_SIZE];
    GPSCoordinates coords;
};

static Units handle_units(Units unit)
{
    if (unit < KELVIN)
    {
        return 0;
    } else if (unit > METRIC)
    {
        return METRIC;
    }

    return unit;
}

static char* units_to_str(Units unit)
{
    switch (unit)
    {
        case IMPERIAL: return UNITS_IMPERIAL;
        case METRIC: return UNITS_METRIC;
        default: return UNITS_KELVIN;
    }
}

int weather_init_api(WeatherAPI* w_api, char* key, Units unit, float latitude, float longitude)
{
    if (w_api == NULL || key == NULL)
    {
        return -1;
    }

    WeatherConfig* cfg = malloc(sizeof(WeatherConfig));
    if (cfg == NULL)
    {
        return -1;
    }

    strncpy(cfg->api, key, API_KEY_SIZE);
    cfg->unit = handle_units(unit);
    cfg->coords.latitude = latitude;
    cfg->coords.longitude = longitude;
    w_api->_config = cfg;

    char url[NET_MAX_URL_LENGTH];
    sprintf(url, API_URL_TEMPLATE, cfg->coords.latitude,
                                             cfg->coords.longitude,
                                             cfg->api,
                                             units_to_str(cfg->unit));

    int res = net_init_handler(&w_api->net_handler, url);
    if (res == -1)
    {
        free(cfg);
        return res;
    }

    return 0;
}

void weather_free_api(WeatherAPI* w_api)
{
    net_free_handler(&w_api->net_handler);
    free(w_api->_config);

    w_api->_config = NULL;
}

int weather_get_forecast(WeatherAPI* w_api, WeatherForecast* w_fc)
{
    if (w_api == NULL || w_fc == NULL)
    {
        return -1;
    }

    cJSON* json = net_get_json_request(&w_api->net_handler);
    if (json == NULL)
    {
        return -1;
    }

    int res = forecast_parse_full(w_fc, json);
    cJSON_Delete(json);

    return res;
}