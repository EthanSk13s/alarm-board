#include <string.h>
#include <stdio.h>

#include "forecasts.h"
#include "cJSON/cJSON.h"

static int forecast_parse_wind(const cJSON* wind_json, WindInfo* wind_info)
{
    const cJSON* wind_deg = cJSON_GetObjectItemCaseSensitive(wind_json, "wind_deg");
    const cJSON* wind_speed = cJSON_GetObjectItemCaseSensitive(wind_json, "wind_speed");
    const cJSON* wind_gust = cJSON_GetObjectItemCaseSensitive(wind_json, "wind_gust");

    if (!cJSON_IsNumber(wind_deg) || !cJSON_IsNumber(wind_speed))
    {
        return -1;
    }

    wind_info->deg = wind_deg->valueint;
    wind_info->speed = wind_speed->valuedouble;
    if (cJSON_IsNumber(wind_gust))
    {
        wind_info->gust = wind_gust->valuedouble;
    }

    return 0;
}

static int forecast_parse_weather(const cJSON* wt_json, Weather* weather)
{
    const cJSON* wts = cJSON_GetObjectItemCaseSensitive(wt_json, "weather");
    const cJSON* wt;
    int i = 0;
    cJSON_ArrayForEach(wt, wts)
    {
        cJSON* id = cJSON_GetObjectItemCaseSensitive(wt, "id");
        cJSON* icon = cJSON_GetObjectItemCaseSensitive(wt, "icon");
        cJSON* main = cJSON_GetObjectItemCaseSensitive(wt, "main");
        if (i < FORECAST_MAX_WEATHER_SIZE)
        {
            if (!cJSON_IsNumber(id) || !cJSON_IsString(icon) || !cJSON_IsString(main))
            {
                return -1;
            }

            weather[i].id = id->valueint;
            strncpy(weather[i].icon, icon->valuestring, FORECAST_MAX_ICON_LENGTH);
            strncpy(weather[i].main, main->valuestring, FORECAST_MAX_WEATHER_NAME_LENGTH);
        } else
        {
            return 0;
        }
        i++;
    }

    return 0;
}

int forecast_parse_current(CurrentForecast* current_fc, const cJSON* curr_json)
{
    const cJSON* current = cJSON_GetObjectItemCaseSensitive(curr_json, "current");
    if (!cJSON_IsObject(current))
    {
        return -1;
    }
    
    const cJSON* temp = cJSON_GetObjectItemCaseSensitive(current, "temp");
    const cJSON* feels_like = cJSON_GetObjectItemCaseSensitive(current, "feels_like");

    if (!cJSON_IsNumber(temp) || !cJSON_IsNumber(feels_like))
    {
        return -1;
    }

    current_fc->temp = temp->valuedouble;
    current_fc->feels_like = feels_like->valuedouble;

    int res = forecast_parse_wind(current, &current_fc->wind);

    if (res == -1)
    {
        return res;
    }

    res = forecast_parse_weather(current, current_fc->weather);
    if (res == -1)
    {
        return res;
    }

    return res;
}

int forecast_parse_dailies(DailyForecast* daily_fc, const cJSON* daily_json)
{
    const cJSON* forecasts = cJSON_GetObjectItemCaseSensitive(daily_json, "daily");
    const cJSON* forecast;
    int i = 0, res = 0;
    cJSON_ArrayForEach(forecast, forecasts)
    {
        if (i < FORECAST_MAX_DAILY_SIZE)
        {
            cJSON* temp = cJSON_GetObjectItemCaseSensitive(forecast, "temp");
            if (!cJSON_IsObject(temp))
            {
                return -1;
            }

            cJSON* temp_max = cJSON_GetObjectItemCaseSensitive(temp, "max");
            cJSON* temp_min = cJSON_GetObjectItemCaseSensitive(temp, "min");

            if (!cJSON_IsNumber(temp_max) || !cJSON_IsNumber(temp_min))
            {
                return -1;
            }

            daily_fc[i].temp_max = temp_max->valuedouble;
            daily_fc[i].temp_min = temp_min->valuedouble;

            res = forecast_parse_wind(forecast, &daily_fc[i].wind);
            if (res == -1)
            {
                return res;
            }

            cJSON* dt = cJSON_GetObjectItemCaseSensitive(forecast, "dt");
            cJSON* sunrise = cJSON_GetObjectItemCaseSensitive(forecast, "sunrise");
            cJSON* sunset = cJSON_GetObjectItemCaseSensitive(forecast, "sunset");

            if (!cJSON_IsNumber(dt) || !cJSON_IsNumber(sunrise) || !cJSON_IsNumber(sunset))
            {
                return -1;
            }

            daily_fc[i].day.dt = dt->valueint;
            daily_fc[i].day.sunrise = sunrise->valueint;
            daily_fc[i].day.sunset = sunset->valueint;

            cJSON* summary = cJSON_GetObjectItemCaseSensitive(forecast, "summary");
            if (!cJSON_IsString(summary))
            {
                return -1;
            }

            strncpy(daily_fc[i].summary, summary->valuestring, FORECAST_MAX_SUMMARY_SIZE);
            res = forecast_parse_weather(forecast, daily_fc[i].weather);
            if (res == -1)
            {
                return res;
            }
        } else
        {
            return 0;
        }

        i++;
    }

    return 0;
}

int forecast_parse_full(WeatherForecast* weather_fc, const cJSON* json)
{
    if (weather_fc == NULL || json == NULL)
    {
        return -1;
    }

    const cJSON* tz_offset = cJSON_GetObjectItemCaseSensitive(json, "timezone_offset");
    if (!cJSON_IsNumber(tz_offset))
    {
        return -1;
    }

    weather_fc->tz_offset = tz_offset->valueint;

    int res = forecast_parse_current(&weather_fc->current, json);
    if (res == -1)
    {
        return res;
    }

    res = forecast_parse_dailies(weather_fc->daily, json);
    if (res == -1)
    {
        return res;
    }

    return res;
}