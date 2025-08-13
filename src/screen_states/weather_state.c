#include <pthread.h>
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "weather_state.h"
#include "clock_state.h"
#include "../timer.h"
#include "../info_storage.h"
#include "../button.h"
#include "../managers/sprite_man.h"
#include "../managers/ui_man.h"
#include "../widgets/forecast_widget.h"

#define DISABLED_TEXT "Functionality is disabled. Check if key is correct."

struct ForecastData
{
    int is_disabled;
    int wthr_new_data;
    int has_loaded;
    pthread_mutex_t lock;
    SpriteManager* sprite_manager;
    Button* clk_btn;
    UIManager ui_manager;
    WeatherForecast fc;
    ForecastWidget fc_wdgt[FORECAST_MAX_DAILY_SIZE];
};

static struct ForecastData forecast_data;
static volatile int wthr_kill_thread = 0;

static void clk_btn_callback();

static void forecast_update(ScreenStatePtr state)
{
    ui_man_poll(&forecast_data.ui_manager);
    if (forecast_data.clk_btn->is_pressed)
    {
        forecast_data.clk_btn->is_pressed = 0;
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
    if (forecast_data.is_disabled)
    {
        Vector2 center = MeasureTextEx(GetFontDefault(), DISABLED_TEXT, 32, 4);
        DrawText(DISABLED_TEXT,
                 ((float) get_current_width() / 2) - (center.x / 2),
                 ((float) get_current_height() / 2) - (center.y / 2), 32, RED);
    }
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
                                              NULL,
                                              KELVIN);
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

    if (!forecast_data.has_loaded)
    {
        ui_man_init(&forecast_data.ui_manager);

        Texture2D* clk_texture = texture_manager_get(get_texture_man(), "clock");

        float width = get_current_width();
        float height = get_current_height();
        Sprite* clk_sprite = create_sprite(width - 180,
                                           height / 1.35,
                                           clk_texture,
                                           (TextureSet) { 10, 0 },
                                           WHITE,
                                           0);
        
        add_to_sprite_manager(forecast_data.sprite_manager, clk_sprite);
        Button* clk_btn = malloc(sizeof(Button));

        btn_init(clk_btn, clk_sprite, clk_btn_callback);
        ui_man_add(&forecast_data.ui_manager, clk_btn);

        forecast_data.clk_btn = clk_btn;

        forecast_data.has_loaded = 1;
    }
}

void* wthr_state_update_thread(void* config_data)
{
    if (config_data == NULL)
    {
        forecast_data.is_disabled = 1;
        printf("WARN: Weather configuration is null. Disabling forecasts.\n");
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
        forecast_data.is_disabled = 1;
        printf("WARN: API key is invalid. Please check if it's correct.\n");
        pthread_exit(NULL);
    }

    for (int i = 0; i < FORECAST_MAX_DAILY_SIZE; i++)
    {
        forecast_data.fc_wdgt[i].unit = cfg->unit;
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

static void clk_btn_callback()
{
    forecast_data.clk_btn->is_pressed = 1;
}