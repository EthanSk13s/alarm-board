#include <pthread.h>
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "weather_state.h"

#include "../timer.h"
#include "../info_storage.h"
#include "../button.h"
#include "../managers/sprite_man.h"
#include "../managers/ui_man.h"
#include "../widgets/forecast_widget.h"
#include "../widgets/template_buttons.h"

#define DISABLED_TEXT "Functionality is disabled. Check if key is correct."

struct ForecastData
{
    int is_disabled;
    int wthr_new_data;
    int has_loaded;
    int crs_btn_id;
    int menu_btn_id;
    int frcst_sprite_id;
    ForecastWidget* slctd_wdgt;
    SpriteManager* sprite_manager;
    pthread_mutex_t lock;
    UIManager ui_manager;
    WeatherForecast fc;
    ForecastWidget fc_wdgt[FORECAST_MAX_DAILY_SIZE];
    int fc_wdgt_ids[FORECAST_MAX_DAILY_SIZE];
};

static struct ForecastData forecast_data;
static volatile int wthr_kill_thread = 0;

static void frcst_wdgt_btn_callback(void* data);
static void crs_btn_callback(void* data);

static void forecast_update(ScreenStatePtr state)
{
    ui_man_poll(&forecast_data.ui_manager);

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

    if (forecast_data.slctd_wdgt != NULL)
    {
        float width = get_current_width();
        float height = get_current_height();
        draw_text_with_rect_bound(GetFontDefault(),
                                  forecast_data.slctd_wdgt->summary,
                                  (Rectangle) {(width / 2) + 100,
                                                   (height / 2) - 100,
                                                   500,
                                                   400 },
                                  32,
                                  4,
                                  1,
                                  RED);
        
        const int text_size = 20;
        char sunrise_txt[text_size];
        char sunset_txt[text_size];


        // Take the current time here to compare the day to the sunrise.
        // We measure here to preserve the inner state for time functions.
        time_t now = time(NULL);
        int curr = localtime(&now)->tm_mday;

        struct tm* sunrise = localtime(&forecast_data.slctd_wdgt->sunrise);
        strftime(sunrise_txt,
                 text_size,
                 "Sunrise - %H:%M",
                 sunrise);
        strftime(sunset_txt,
                 text_size,
                 "Sunset - %H:%M",
                 localtime(&forecast_data.slctd_wdgt->sunset));

        int pos_x = width / 10;
        int pos_y = (height / 2);
        int font_size = 32;
        if (curr == sunrise->tm_mday)
        {
            char curr_temp[text_size];
            snprintf(curr_temp,
                     text_size,
                     "Feels Like: %.2f %c\n",
                     forecast_data.fc.current.feels_like,
                     utils_get_temp_unit_symbol(IMPERIAL));
            
            DrawText(curr_temp,
                     pos_x,
                     pos_y - 300,
                     font_size,
                     RED);
        }

        DrawText(sunrise_txt,
                 pos_x,
                 pos_y + 250,
                 font_size,
                 RED);
        DrawText(sunset_txt,
                 pos_x,
                 pos_y + 300,
                 font_size,
                 RED);
    }

    draw_sprites(forecast_data.sprite_manager);
}

void wthr_state_init()
{
    forecast_data.sprite_manager = create_sprite_manager();
    ui_man_init(&forecast_data.ui_manager);

    int x = 50; int y = get_current_height() / 4;
    for (int i = 0; i < FORECAST_MAX_DAILY_SIZE; i++)
    {
        Sprite* forecast = wdgt_forecast_init(&forecast_data.fc_wdgt[i],
                                              (Vector2) {x, y},
                                              (Vector2) {150, 400},
                                              NULL,
                                              KELVIN);
        int id = add_to_sprite_manager(forecast_data.sprite_manager, forecast);
        forecast_data.fc_wdgt_ids[i] = id;

        Button* button = malloc(sizeof(Button));
        if (button == NULL)
        {
            break;
        }

        btn_init(button, forecast, frcst_wdgt_btn_callback, &forecast_data.fc_wdgt[i]);
        ui_man_add(&forecast_data.ui_manager, button);
        x += 250;
    }
}

void clean_up_wthr_state()
{
    for (int i = 0; i < FORECAST_MAX_DAILY_SIZE; i++)
    {
        wdgt_forecast_free(&forecast_data.fc_wdgt[i]);
    }

    Button* curr_btn = ui_man_pop(&forecast_data.ui_manager);
    while(curr_btn)
    {
        free(curr_btn);
        curr_btn = ui_man_pop(&forecast_data.ui_manager);
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
    forecast_data.slctd_wdgt = NULL;

    if (!forecast_data.has_loaded)
    {
        Texture2D* cross_texture = texture_manager_get(get_texture_man(), "cross");
        Texture2D* unkwn_texture = texture_manager_get(get_texture_man(), "unknown");

        float width = get_current_width();
        float height = get_current_height();
        TextureSet btn_scales = (TextureSet) { 10, 0 };

        Sprite* crs_sprite = create_sprite(width - 180,
                                           height / 1.35,
                                           cross_texture,
                                           btn_scales,
                                           WHITE,
                                           0);
        Sprite* crnt_frcst_sprite = create_sprite(5,
                                                  50,
                                                  unkwn_texture,
                                                  (TextureSet) { 35, 0 },
                                                  RED,
                                                  0);

        forecast_data.menu_btn_id = button_menu(get_texture_man(),
                                                forecast_data.sprite_manager,
                                                &forecast_data.ui_manager,
                                                state,
                                                0,
                                                20);

        crs_sprite->visible = 0;
        forecast_data.crs_btn_id = add_to_sprite_manager(forecast_data.sprite_manager,
                                                         crs_sprite);
        crnt_frcst_sprite->visible = 0;
        forecast_data.frcst_sprite_id = add_to_sprite_manager(forecast_data.sprite_manager,
                                                              crnt_frcst_sprite);

        Button* crs_btn = malloc(sizeof(Button));
        Button* menu_btn = malloc(sizeof(Button));

        btn_init(crs_btn, crs_sprite, crs_btn_callback, NULL);
        ui_man_add(&forecast_data.ui_manager, crs_btn);

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

static void frcst_wdgt_btn_callback(void* data)
{
    ForecastWidget* fc_wdgt = (ForecastWidget*) data;
    toggle_sprite_visibility(forecast_data.sprite_manager, forecast_data.crs_btn_id);
    toggle_sprite_visibility(forecast_data.sprite_manager, forecast_data.menu_btn_id);
    toggle_sprite_visibility(forecast_data.sprite_manager, forecast_data.frcst_sprite_id);

    for (int i = 0; i < FORECAST_MAX_DAILY_SIZE; i++)
    {
        toggle_sprite_visibility(forecast_data.sprite_manager,
                                 forecast_data.fc_wdgt_ids[i]);
    }

    Texture2D* new_texture = texture_manager_get(get_texture_man(),
                                                 parse_wthr_icon(fc_wdgt->icon_id));
    if (new_texture == NULL)
    {
        new_texture = texture_manager_get(get_texture_man(), "unknown");
    }
    sprite_man_swap_textures(forecast_data.sprite_manager,
                             forecast_data.frcst_sprite_id,
                             new_texture);

    forecast_data.slctd_wdgt = fc_wdgt;
}

static void crs_btn_callback(void* data)
{
    for (int i = 0; i < FORECAST_MAX_DAILY_SIZE; i++)
    {
        toggle_sprite_visibility(forecast_data.sprite_manager, forecast_data.fc_wdgt_ids[i]);
    }

    toggle_sprite_visibility(forecast_data.sprite_manager, forecast_data.menu_btn_id);
    toggle_sprite_visibility(forecast_data.sprite_manager, forecast_data.crs_btn_id);
    toggle_sprite_visibility(forecast_data.sprite_manager, forecast_data.frcst_sprite_id);

    forecast_data.slctd_wdgt = NULL;
}