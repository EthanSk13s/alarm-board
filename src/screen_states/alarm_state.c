#include <time.h>
#include <raylib.h>

#include "alarm_state.h"
#include "clock_state.h"
#include "screen_state.h"

#include "../sprite_man.h"
#include "../info_storage.h"
#include "../button.h"

struct AlarmData
{
    char alarm[6];
    int textures_loaded;
    SpriteManager* sprite_manager;
    struct tm alarm_tm;
    struct Button hour_button_dec;
    struct Button hour_button_inc;
    struct Button minute_button_inc;
    struct Button minute_button_dec;
    struct Button clock_button;
};

static struct AlarmData alarm_data;

static void update_alarm()
{
    alarm_data.alarm_tm = get_alarm();
    strftime(alarm_data.alarm, 6, "%H:%M", &alarm_data.alarm_tm);
}

static void alarm_update(ScreenStatePtr state)
{
    if (IsKeyPressed(KEY_A) || check_pressed(&alarm_data.clock_button))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsGestureDetected(GESTURE_TAP))
        {
            alarm_data.clock_button.is_pressed = true;
        }
    }

    if (check_pressed(&alarm_data.hour_button_inc))
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            int inc = alarm_data.alarm_tm.tm_hour + 1;
            set_alarm_hour(inc);
            update_alarm();
        }
    }

    if (check_pressed(&alarm_data.hour_button_dec))
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            int dec = alarm_data.alarm_tm.tm_hour - 1;
            if (dec <= -1)
            {
                dec = 23;
            }
            set_alarm_hour(dec);
            update_alarm();
        }
    }

    if (check_pressed(&alarm_data.minute_button_inc))
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            int inc = alarm_data.alarm_tm.tm_min + 1;
            set_alarm_min(inc);
            update_alarm();
        }
    }

    if (check_pressed(&alarm_data.minute_button_dec))
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            int dec = alarm_data.alarm_tm.tm_min - 1;
            if (dec <= -1)
            {
                dec = 59;
            }
            set_alarm_min(dec);
            update_alarm();
        }
    }

    if (alarm_data.clock_button.is_pressed)
    {
        alarm_data.clock_button.is_pressed = false;
        transition_to_clock(state);
    }
}

static void alarm_draw(ScreenStatePtr state)
{
    ClearBackground(BLACK);

    Vector2 center = MeasureTextEx(GetFontDefault(), alarm_data.alarm, CLOCK_FONT_SIZE, 8);
    DrawText(alarm_data.alarm,
                ((float) get_current_width() / 2) - (center.x / 2),
                ((float) get_current_height() / 2) - (center.y / 2),
                CLOCK_FONT_SIZE, RED);
    draw_sprites(alarm_data.sprite_manager);
}

void transition_to_alarm(ScreenStatePtr state)
{
    default_state(state);
    state->update = alarm_update;
    state->draw = alarm_draw;
    update_alarm();

    Vector2 center = MeasureTextEx(GetFontDefault(), alarm_data.alarm, CLOCK_FONT_SIZE, 8);

    int width = get_current_width();
    int height = get_current_height();

    if (!alarm_data.textures_loaded)
    {
        alarm_data.sprite_manager = create_sprite_manager();
        Texture2D* arrow_texture = texture_manager_get(get_texture_man(), "alarm-arrow");
        Texture2D* clock_texture = texture_manager_get(get_texture_man(), "clock");

        TextureSet clock_opts = { 10, 0 };
        TextureSet upper_opts = { 2, 0 };
        TextureSet lower_opts = { 2, 180 };

        Sprite* hour_btn_dec_sprite = create_sprite((float) width / 2 - (center.x / 3.25),
                                                    (float) height / 2 + (center.y / 2),
                                                    arrow_texture,
                                                    lower_opts,
                                                    BLUE);
        Sprite* hour_btn_inc_sprite = create_sprite((float) width / 2 - (center.x / 3.25),
                                                    (float) height / 2 - (center.y / 1.25),
                                                    arrow_texture,
                                                    upper_opts,
                                                    BLUE);
        Sprite* minute_btn_dec_sprite = create_sprite((float) width / 2 + (center.x / 3.25),
                                                      (float) height / 2 + (center.y / 2),
                                                      arrow_texture,
                                                      lower_opts,
                                                      BLUE);
        Sprite* minute_btn_inc_sprite = create_sprite((float) width / 2 + (center.x / 3.25),
                                                      (float) height / 2 - (center.y / 1.25),
                                                      arrow_texture,
                                                      upper_opts,
                                                      BLUE);
        Sprite* clock_sprite = create_sprite(width - 180,
                                             height / 1.5,
                                             clock_texture,
                                             clock_opts,
                                             WHITE);

        add_to_sprite_manager(alarm_data.sprite_manager, hour_btn_dec_sprite);
        add_to_sprite_manager(alarm_data.sprite_manager, hour_btn_inc_sprite);
        add_to_sprite_manager(alarm_data.sprite_manager, minute_btn_dec_sprite);
        add_to_sprite_manager(alarm_data.sprite_manager, minute_btn_inc_sprite);
        add_to_sprite_manager(alarm_data.sprite_manager, clock_sprite);

        alarm_data.hour_button_dec = create_button(hour_btn_dec_sprite);
        alarm_data.hour_button_inc = create_button(hour_btn_inc_sprite);

        alarm_data.minute_button_dec = create_button(minute_btn_dec_sprite);
        alarm_data.minute_button_inc = create_button(minute_btn_inc_sprite);

        alarm_data.clock_button = create_button(clock_sprite);
    }
}