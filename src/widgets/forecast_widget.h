#ifndef FORECAST_WIDGET_H
#define FORECAST_WIDGET_H

#include <raylib.h>

#include "../sprite.h"
#include "../weather/api.h"
#include "../weather/forecasts.h"

typedef struct
{
    Units unit;
    float temp_max;
    float temp_min;
    char* summary;
    char icon_id[FORECAST_MAX_ICON_LENGTH];
    time_t dt;
    time_t sunrise;
    time_t sunset;
    Vector2 pos;
    Vector2 size;
    RenderTexture2D target;
} ForecastWidget;

// Pass a NULL to forecast if you don't need the data after initialization right away.
Sprite* wdgt_forecast_init(ForecastWidget* fc_wdgt, Vector2 pos, Vector2 size, DailyForecast* forecast, Units unit);
void wdgt_forecast_free(ForecastWidget* fc_wdgt);
int wdgt_forecast_update(ForecastWidget* fc_wdgt, DailyForecast* forecast);

#endif