#include <pthread.h>
#include <raylib.h>
#include <stdio.h>
#include <unistd.h>

#include "weather_state.h"

#include "clock_state.h"
#include "../timer.h"
#include "../info_storage.h"
#include "../managers/sprite_man.h"
#include "../widgets/forecast_widget.h"


struct ForecastData
{
    int wthr_new_data;
    pthread_mutex_t lock;
    SpriteManager* sprite_manager;
    WeatherForecast fc;
    ForecastWidget fc_wdgt[FORECAST_MAX_DAILY_SIZE];
};

static struct ForecastData forecast_data;
static volatile int wthr_kill_thread = 0;

static void forecast_update(ScreenStatePtr state)
{
    if (IsKeyPressed(KEY_W))
    {
        transition_to_clock(state);
    }

    if (forecast_data.wthr_new_data == 1)
    {
        for (int i = 0; i < FORECAST_MAX_DAILY_SIZE; i++)
        {
            wdgt_forecast_update(&forecast_data.fc_wdgt[i], &forecast_data.fc.daily[i]);
        }

        forecast_data.wthr_new_data = 0;
    }
}

static void forecast_draw(ScreenStatePtr state)
{
    ClearBackground(BLACK);
    draw_sprites(forecast_data.sprite_manager);
}

void wthr_state_init()
{
    forecast_data.sprite_manager = create_sprite_manager();

    int x = 50; int y = get_current_height() / 4;
    for (int i = 0; i < FORECAST_MAX_DAILY_SIZE; i++)
    {
        Sprite* forecast = wdgt_forecast_init(&forecast_data.fc_wdgt[i],
                                              (Vector2) {x, y},
                                              (Vector2) {150, 400},
                                              NULL);
        add_to_sprite_manager(forecast_data.sprite_manager, forecast);
        x += 250;
    }
}

void clean_up_wthr_state()
{
    for (int i = 0; i < FORECAST_MAX_DAILY_SIZE; i++)
    {
        wdgt_forecast_free(&forecast_data.fc_wdgt[i]);
    }
    sprite_man_free(forecast_data.sprite_manager);
}

void wthr_state_kill_thread()
{
    wthr_kill_thread = 1;
}

void transition_to_wthr_state(ScreenStatePtr state)
{
    default_state(state);
    state->draw = forecast_draw;
    state->update = forecast_update;
}

void* wthr_state_update_thread(void* config_data)
{
    if (config_data == NULL)
    {
        pthread_exit(NULL);
    }

    WeatherStateConfig* cfg = (WeatherStateConfig*) config_data;
    WeatherAPI w_api;
    int res = weather_init_api(&w_api,
                               cfg->key,
                               cfg->unit,
                               cfg->latitude,
                               cfg->longitude);
    if (res == -1)
    {
        pthread_exit(NULL);
    }

    Timer update_timer;
    timer_start(&update_timer, 60 * 60);
    while(!wthr_kill_thread)
    {
        res = weather_get_forecast(&w_api, &forecast_data.fc);
        if (res == -1)
        {
            printf("WARN: Failed to get current data.\n");
        } else
        {
            pthread_mutex_lock(&forecast_data.lock);
            forecast_data.wthr_new_data = 1;
            pthread_mutex_unlock(&forecast_data.lock);
        }

        // Sleep for a second to save on CPU cycles.
        while (!timer_done(update_timer) && !wthr_kill_thread) sleep(1);
        timer_start(&update_timer, 60 * 60);
    }

    weather_free_api(&w_api);
    return NULL;
}