#include <string.h>
#include <time.h>

#include "alarm_state.h"
#include "raylib.h"

#include "../button.h"
#include "../info_storage.h"
#include "../alarm.h"
#include "../utils.h"
#include "../sprite.h"
#include "../sprite_man.h"
#include "../timer.h"

#include "screen_state.h"
#include "clock_state.h"

#define SNOOZE_TIME 600.0

struct ClockData
{
    int play_alarm;
    int textures_loaded;
    struct Button alarm_button;
    struct Button toggle_button;
    struct Button snooze_button;
    SpriteManager* sprite_manager;
    Timer snooze_timer;
    int snooze_id;
    char clock[26];
    char alarm_cmp[26];
};

static struct ClockData clock_data;

static void clock_update(ScreenStatePtr state)
{
    time_t timer;
    struct tm* tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(clock_data.clock, 26, "%H:%M", tm_info);

    if (check_pressed(&clock_data.alarm_button))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsGestureDetected(GESTURE_TAP))
        {
            clock_data.alarm_button.is_pressed = true;
        }
    }

    if (check_pressed(&clock_data.toggle_button))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsGestureDetected(GESTURE_TAP))
        {
            if (clock_data.toggle_button.is_pressed)
            {
                clock_data.toggle_button.is_pressed = false;
                clock_data.snooze_button.is_pressed = false;
                clock_data.play_alarm = 0;

                if (is_alarm_playing())
                {
                    toggle_sprite_visibility(clock_data.sprite_manager, clock_data.snooze_id);
                    stop_alarm();
                }
            } else
            {
                clock_data.toggle_button.is_pressed = true;
            }
        }
    }

    if (check_pressed(&clock_data.snooze_button))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsGestureDetected(GESTURE_TAP))
        {
            clock_data.snooze_button.is_pressed = true;
            clock_data.play_alarm = 0;
            timer_start(&clock_data.snooze_timer, SNOOZE_TIME);
            stop_alarm();
            toggle_sprite_visibility(clock_data.sprite_manager, clock_data.snooze_id);
        }
    }

    if (timer_done(clock_data.snooze_timer) && !clock_data.play_alarm && clock_data.snooze_button.is_pressed)
    {
        clock_data.play_alarm = 1;
    }

    if (clock_data.alarm_button.is_pressed)
    {
        clock_data.alarm_button.is_pressed = false;
        transition_to_alarm(state);
    }

    if (clock_data.toggle_button.is_pressed)
    {
        if (!clock_data.snooze_button.is_pressed)
        {
            if (strcmp(clock_data.clock, clock_data.alarm_cmp) == 0 && clock_data.play_alarm == 0)
            {
                clock_data.play_alarm = 1;
            }
        }

        if (clock_data.play_alarm && !is_alarm_playing())
        {
            play_alarm();
            toggle_sprite_visibility(clock_data.sprite_manager, clock_data.snooze_id);
        }
    }
}

static void clock_draw(ScreenStatePtr state)
{
    ClearBackground(BLACK);

    Vector2 center = MeasureTextEx(GetFontDefault(), clock_data.clock, CLOCK_FONT_SIZE, 8);
    DrawText(clock_data.clock,
                ((float) get_current_width() / 2) - (center.x / 2),
                ((float) get_current_height() / 2) - (center.y / 2), CLOCK_FONT_SIZE, RED);
    
    draw_sprites(clock_data.sprite_manager);

    /*
    if (clock_data.play_alarm == 1)
    {
        draw_button(&clock_data.snooze_button);
    }
    */

    // Note: Temporary status toggle.
    if (clock_data.toggle_button.is_pressed)
    {
        DrawRectangle(100,
                      (float) (get_current_height() / 1.5),
                      10,
                      10,
                      RED);
    }
}

void transition_to_clock(ScreenStatePtr state)
{
    default_state(state);
    state->draw = clock_draw;
    state->update = clock_update;

    clock_data.play_alarm = 0;
    struct tm alarm_tm = get_alarm();
    strftime(clock_data.alarm_cmp, 26, "%H:%M", &alarm_tm);

    if (!clock_data.textures_loaded)
    {
        Vector2 center = MeasureTextEx(GetFontDefault(), clock_data.alarm_cmp, CLOCK_FONT_SIZE, 8);
        clock_data.sprite_manager = create_sprite_manager();
        Texture2D* alarm_texture = texture_manager_get(get_texture_man(), "alarm");
        Texture2D* toggle_texture = texture_manager_get(get_texture_man(), "set-alarm");
        Texture2D* snooze_texture = texture_manager_get(get_texture_man(), "snooze");

        TextureSet alarm_toggle_texture_opts = { 10 , 0};
        float width = get_current_width();
        float height = get_current_height();

        Sprite* alarm_sprite = create_sprite(width - 180,
                                             height / 1.5,
                                             alarm_texture,
                                             alarm_toggle_texture_opts,
                                             WHITE);
        Sprite* toggle_sprite = create_sprite(0,
                                              height / 1.5,
                                              toggle_texture,
                                              alarm_toggle_texture_opts,
                                              RED);
        Sprite* snooze_sprite = create_sprite(center.x,
                                              height / 1.5,
                                              snooze_texture,
                                              alarm_toggle_texture_opts,
                                              BLUE);
        
        add_to_sprite_manager(clock_data.sprite_manager, alarm_sprite);
        add_to_sprite_manager(clock_data.sprite_manager, toggle_sprite);

        clock_data.snooze_id = add_to_sprite_manager(clock_data.sprite_manager,
                                                     snooze_sprite);

        toggle_sprite_visibility(clock_data.sprite_manager, clock_data.snooze_id);
        clock_data.alarm_button = create_button(alarm_sprite);
        clock_data.snooze_button = create_button(snooze_sprite);
        clock_data.toggle_button = create_button(toggle_sprite);

        clock_data.textures_loaded = 1;
    }

}