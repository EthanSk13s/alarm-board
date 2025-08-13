#include <raylib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "forecast_widget.h"
#include "../managers/texture_man.h"
#include "../info_storage.h"
#include "../utils.h"

#define WEEKDAY_FONT_SIZE 32
#define TEMP_FONT_SIZE    20

#define ICON_SCALE 10

static char* parse_wthr_icon(char* icon_id);
static char get_temp_unit_symbol(Units unit);

static void wdgt_forecast_draw(ForecastWidget* fc_wdgt)
{
    Texture2D* icon = texture_manager_get(get_texture_man(),
                                          parse_wthr_icon(fc_wdgt->icon_id));
    
    const int weekday_size = 6;
    char weekday[weekday_size];
    struct tm* fc_day = localtime(&fc_wdgt->dt);
    int fc_wkday = fc_day->tm_wday;

    time_t now = time(NULL);
    int curr_wday = localtime(&now)->tm_wday;
    if (fc_wkday == curr_wday)
    {
        strncpy(weekday, "Today", weekday_size);
    } else
    {
        strftime(weekday, weekday_size, "%a", localtime(&fc_wdgt->dt));
    }

    // Here we process the string for the temperature,
    // and center it based on the widget's dimensions.
    const int temp_size = 15;

    char temp_low[temp_size];
    char temp_high[temp_size];
    char temp_symbol = get_temp_unit_symbol(fc_wdgt->unit);
    snprintf(temp_low, temp_size, "Min:  %.2f %c", fc_wdgt->temp_min, temp_symbol);
    snprintf(temp_high, temp_size, "Max: %.2f %c", fc_wdgt->temp_max, temp_symbol);

    Vector2 center_temp = utils_center_text(GetFontDefault(),
                                            fc_wdgt->size,
                                            temp_low,
                                            TEMP_FONT_SIZE,
                                            2);
    Vector2 center_day = utils_center_text(GetFontDefault(),
                                           fc_wdgt->size,
                                           weekday,
                                           WEEKDAY_FONT_SIZE,
                                           0);

    // Here we position the icon in the center of the widget.
    Vector2 origin = {
                       icon->width * ICON_SCALE / 2.0f,
                       icon->height * ICON_SCALE / 2.0f
                     };
    Rectangle src = { 0, 0, (float) icon->width, (float) icon->height };
    Rectangle dest = {
                       fc_wdgt->size.x / 2,
                       (fc_wdgt->size.y / 2) - 25,
                       (float) icon->width * ICON_SCALE,
                       (float) icon->height * ICON_SCALE
                     };

    BeginTextureMode(fc_wdgt->target);
        ClearBackground(BLACK);
        DrawText(weekday, center_day.x, 0, WEEKDAY_FONT_SIZE, RED);
        DrawTexturePro(*icon, src, dest, origin, 0.0f, RED);

        DrawText(temp_high,
                 center_temp.x,
                 fc_wdgt->size.y - fc_wdgt->size.y / 3,
                 TEMP_FONT_SIZE,
                 RED);
        DrawText(temp_low,
                 center_temp.x,
                 fc_wdgt->size.y - fc_wdgt->size.y / 4.5,
                 TEMP_FONT_SIZE,
                 RED);
    EndTextureMode();
}

Sprite* wdgt_forecast_init(ForecastWidget* fc_wdgt, Vector2 pos, Vector2 size, DailyForecast* forecast, Units unit)
{
    if (fc_wdgt == NULL)
    {
        return NULL;
    }

    fc_wdgt->pos = pos;
    fc_wdgt->size = size;
    fc_wdgt->target = LoadRenderTexture(fc_wdgt->size.x, fc_wdgt->size.y);
    fc_wdgt->unit = unit;
    if (forecast != NULL)
    {
        int res = wdgt_forecast_update(fc_wdgt, forecast);
        if (res == -1)
        {
            return NULL;
        }
    }

    return create_sprite(fc_wdgt->pos.x,
                         fc_wdgt->pos.y,
                         &fc_wdgt->target.texture,
                         (TextureSet) {1, 0},
                         WHITE,
                         1);
}

void wdgt_forecast_free(ForecastWidget* fc_wdgt)
{
    UnloadRenderTexture(fc_wdgt->target);
}

int wdgt_forecast_update(ForecastWidget* fc_wdgt, DailyForecast* forecast)
{
    if (fc_wdgt == NULL || forecast == NULL)
    {
        return -1;
    }

    strncpy(fc_wdgt->icon_id, forecast->weather[0].icon, FORECAST_MAX_ICON_LENGTH);
    fc_wdgt->temp_max = forecast->temp_max;
    fc_wdgt->temp_min = forecast->temp_min;
    fc_wdgt->dt = forecast->day.dt;

    wdgt_forecast_draw(fc_wdgt);
    return 0;
}

static char get_temp_unit_symbol(Units unit)
{
    switch(unit)
    {
        case METRIC: return 'C';
        case IMPERIAL: return 'F';
        default: return 'K';
    }
}

static char* parse_wthr_icon(char* icon_id)
{
    int id = atoi(icon_id);
    char time = icon_id[FORECAST_MAX_ICON_LENGTH - 2];

    char* key;
    switch(id)
    {
        case 1:
        {
            if (time == 'd')
            {
                key = "sun";
            } else
            {
                key = "moon";
            }
        } break;
        case 2:
        {
            if (time == 'd')
            {
                key = "day_clouded";
            } else
            {
                key = "night_clouded";
            }
        } break;
        case 3: key = "scattered"; break;
        case 4: key = "broken"; break;
        case 9: key = "shower"; break;
        case 10:
        {
            if (time == 'd')
            {
                key = "day_rain";
            } else
            {
                key = "night_rain";
            }
        } break;
        case 11: key = "rainstorm"; break;
        case 13: key = "snow"; break;
        case 50: key = "mist"; break;
        default: key = "unknown";
    }

    return key;
}