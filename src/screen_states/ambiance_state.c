#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "ambiance_state.h"

#include "../timer.h"
#include "../info_storage.h"
#include "../managers/sprite_man.h"
#include "../managers/texture_man.h"
#include "../managers/ui_man.h"
#include "../widgets/template_buttons.h"

#define SECONDS_IN_MINUTE 60
#define SECONDS_IN_HOUR 3600

struct AmbData
{
    int textures_loaded;
    int timer_started;
    int timer_val;
    char timer_str[8];
    SpriteManager* sprite_manager;
    int btns_to_hide[10];
    UIManager ui_manager;
};

static void toggle_btns(const struct AmbData* amb_data)
{
    for (int i = 0; i < 6; i++)
    {
        toggle_sprite_visibility(amb_data->sprite_manager,
                                 amb_data->btns_to_hide[i]);
    }
}

static void update_timer_str(char* str, const int timer_val, const int strlen)
{
    if (timer_val >= 0)
    {
        int hour = timer_val / SECONDS_IN_HOUR;
        int min = (timer_val / SECONDS_IN_MINUTE) % SECONDS_IN_MINUTE;

        snprintf(str, strlen, "%02d:%02d", hour, min);
    }
}

static struct AmbData amb_data = { 0 };

static void menu_btn_callback(void* state);
static void rain_btn_callback(void* data);
static void start_btn_callback(void* data);
static void clear_btn_callback(void* data);
static void hour_inc_btn_callback(void* data);
static void hour_dec_btn_callback(void* data);
static void min_inc_btn_callback(void* data);
static void min_dec_btn_callback(void* data);

void amb_update(ScreenStatePtr state)
{
    ui_man_poll(&amb_data.ui_manager);

    if (amb_data.timer_started)
    {
        AmbianceManager* amb_man = get_amb_man();
        if (amb_man_is_done(amb_man))
        {
            toggle_btns(&amb_data);
            amb_data.timer_started = 0;
        }

        update_timer_str(amb_data.timer_str,
                         amb_data.timer_val - timer_get_elapsed(amb_man->amb_timer),
                         8);
    } else
    {
        update_timer_str(amb_data.timer_str, amb_data.timer_val, 8);
    }
}

void amb_draw(ScreenStatePtr state)
{
    ClearBackground(BLACK);

    Vector2 center = MeasureTextEx(GetFontDefault(), amb_data.timer_str, CLOCK_FONT_SIZE, 8);
    DrawText(amb_data.timer_str,
             ((float) get_current_width() / 2) - (center.x / 2),
             ((float) get_current_height() / 2) - (center.y / 2),
             CLOCK_FONT_SIZE,
             WHITE);
    draw_sprites(amb_data.sprite_manager);
}

void transition_to_amb_state(ScreenStatePtr state)
{
    state->update = amb_update;
    state->draw = amb_draw;

    if (amb_data.timer_val <= 0)
    {
        snprintf(amb_data.timer_str, 8, "%02d:%02d", 0, 0);
    }

    Vector2 center = MeasureTextEx(GetFontDefault(), amb_data.timer_str, CLOCK_FONT_SIZE, 8);
    if (!amb_data.textures_loaded)
    {
        amb_data.sprite_manager = create_sprite_manager();
        ui_man_init(&amb_data.ui_manager);

        TextureManager* txt_manager = get_texture_man();
        Texture2D* arrow_txt = texture_manager_get(txt_manager, "alarm-arrow");
        Texture2D* cross_txt = texture_manager_get(txt_manager, "cross");
        Texture2D* rain_txt = texture_manager_get(txt_manager, "day_rain");
        Texture2D* check_txt = texture_manager_get(txt_manager, "check");

        TextureSet btn_opts = { 10 , 0 };
        TextureSet up_arrow_opts = { 4, 0 };
        TextureSet down_arrow_opts = { 4 ,180 };

        int width = get_current_width();
        int height = get_current_height();

        int hour_x = (float) width / 2 - (center.x / 3.25);
        Sprite* hour_btn_dec_sprite = create_sprite(hour_x,
                                                    (float) height / 2 + (center.y / 2),
                                                    arrow_txt,
                                                    down_arrow_opts,
                                                    BLUE,
                                                    0);
        Sprite* hour_btn_inc_sprite = create_sprite(hour_x,
                                                    (float) height / 2 - (center.y / 1.25),
                                                    arrow_txt,
                                                    up_arrow_opts,
                                                    BLUE,
                                                    0);
        
        int min_x = (float) width / 2 + (center.x / 3.25);
        Sprite* minute_btn_dec_sprite = create_sprite(min_x,
                                                      (float) height / 2 + (center.y / 2),
                                                      arrow_txt,
                                                      down_arrow_opts,
                                                      BLUE,
                                                      0);
        Sprite* minute_btn_inc_sprite = create_sprite(min_x,
                                                      (float) height / 2 - (center.y / 1.25),
                                                      arrow_txt,
                                                      up_arrow_opts,
                                                      BLUE,
                                                      0);

        
        Sprite* rain_sprite = create_sprite(0, 180, rain_txt, btn_opts, WHITE, 0);

        float status_y = (float) height / 2 + (center.y / 2) + 60;
        Sprite* check_sprite = create_sprite(min_x,
                                          status_y,
                                          check_txt,
                                          btn_opts,
                                          WHITE,
                                          0);
        Sprite* cross_sprite = create_sprite(hour_x,
                                             (float) height / 2 + (center.y / 2) + 60,
                                             cross_txt,
                                             btn_opts,
                                             WHITE,
                                             0);

        amb_data.btns_to_hide[0] = add_to_sprite_manager(amb_data.sprite_manager,
                                                         hour_btn_dec_sprite);
        amb_data.btns_to_hide[1] = add_to_sprite_manager(amb_data.sprite_manager,
                                                         hour_btn_inc_sprite);
        amb_data.btns_to_hide[2] = add_to_sprite_manager(amb_data.sprite_manager,
                                                         minute_btn_dec_sprite);
        amb_data.btns_to_hide[3] = add_to_sprite_manager(amb_data.sprite_manager,
                                                         minute_btn_inc_sprite);

        amb_data.btns_to_hide[4] = add_to_sprite_manager(amb_data.sprite_manager,
                                                         rain_sprite);
        amb_data.btns_to_hide[5] = add_to_sprite_manager(amb_data.sprite_manager,
                                                         check_sprite);
        
        button_menu(txt_manager,
                    amb_data.sprite_manager,
                    &amb_data.ui_manager,
                    state,
                    0,
                    20);
        
        add_to_sprite_manager(amb_data.sprite_manager, cross_sprite);

        Button* hour_btn_dec = malloc(sizeof(Button));
        Button* hour_btn_inc = malloc(sizeof(Button));
        Button* min_btn_dec = malloc(sizeof(Button));
        Button* min_btn_inc = malloc(sizeof(Button));

        Button* rain_btn = malloc(sizeof(Button));
        Button* cross_btn = malloc(sizeof(Button));
        Button* check_btn = malloc(sizeof(Button));

        btn_init(hour_btn_dec, hour_btn_dec_sprite, hour_dec_btn_callback, NULL);
        btn_init(hour_btn_inc, hour_btn_inc_sprite, hour_inc_btn_callback, NULL);
        btn_init(min_btn_dec, minute_btn_dec_sprite, min_dec_btn_callback, NULL);
        btn_init(min_btn_inc, minute_btn_inc_sprite, min_inc_btn_callback, NULL);

        btn_init(rain_btn, rain_sprite, rain_btn_callback, NULL);
        btn_init(cross_btn, cross_sprite, clear_btn_callback, NULL);
        btn_init(check_btn, check_sprite, start_btn_callback, NULL);

        ui_man_add(&amb_data.ui_manager, hour_btn_dec);
        ui_man_add(&amb_data.ui_manager, hour_btn_inc);
        ui_man_add(&amb_data.ui_manager, min_btn_dec);
        ui_man_add(&amb_data.ui_manager, min_btn_inc);
        ui_man_add(&amb_data.ui_manager, rain_btn);
        ui_man_add(&amb_data.ui_manager, cross_btn);
        ui_man_add(&amb_data.ui_manager, check_btn);

        amb_data.textures_loaded = 1;
    }
}

static void hour_inc_btn_callback(void* data)
{
    amb_data.timer_val += SECONDS_IN_HOUR;
}

static void hour_dec_btn_callback(void* data)
{
    amb_data.timer_val -= SECONDS_IN_HOUR;
}

static void min_inc_btn_callback(void* data)
{
    amb_data.timer_val += SECONDS_IN_MINUTE;
}

static void min_dec_btn_callback(void* data)
{
    amb_data.timer_val -= SECONDS_IN_MINUTE;
}

static void clear_btn_callback(void* data)
{
    amb_data.timer_val = 0;

    if (amb_data.timer_started)
    {
        toggle_btns(&amb_data);
        amb_data.timer_started = 0;

        AmbianceManager* amb_man = get_amb_man();
        amb_man_stop(amb_man);
    }
}

static void start_btn_callback(void* data)
{
    AmbianceManager* amb_man = get_amb_man();
    int res = amb_man_start(amb_man, amb_data.timer_val);

    if (res != -1)
    {
        toggle_btns(&amb_data);
        amb_data.timer_started = 1;
    }
}

static void rain_btn_callback(void* data)
{
    // TODO: Add button feedback.
    AmbianceManager* amb_man = get_amb_man();
    amb_man_switch_to_rain(amb_man);
}