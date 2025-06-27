#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "alarm_state.h"
#include "raylib.h"

#include "../button.h"
#include "../info_storage.h"
#include "../alarm.h"
#include "../utils.h"

#include "screen_state.h"
#include "clock_state.h"


struct ClockData
{
    int play_alarm;
    struct Button alarm_button;
    struct Button toggle_button;
    struct Button snooze_button;
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
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsGestureDetected(GESTURE_TAP))
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
                clock_data.play_alarm = 0;

                if (is_alarm_playing())
                {
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
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsGestureDetected(GESTURE_TAP))
        {
            clock_data.snooze_button.is_pressed = false;
            clock_data.play_alarm = 0;
            // TODO: Wait for 10 Minutes and play alarm again.
        }
    }

    if (clock_data.alarm_button.is_pressed)
    {
        transition_to_alarm(state);
        clock_data.alarm_button.is_pressed = false;
    }

    if (clock_data.toggle_button.is_pressed)
    {
        if (strcmp(clock_data.clock, clock_data.alarm_cmp) == 0 && clock_data.play_alarm == 0)
        {
            clock_data.play_alarm = 1;
        }

        if (clock_data.play_alarm && !is_alarm_playing())
        {
            play_alarm();
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

    draw_button(&clock_data.alarm_button);
    draw_button(&clock_data.toggle_button);

    if (clock_data.play_alarm == 1)
    {
        draw_button(&clock_data.snooze_button);
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

    TextureSet toggle_texture = { "assets/alarm.png", 10 };
    TextureSet alarm_texture = { "assets/set-alarm.png", 10 };

    clock_data.alarm_button = create_button(get_current_width() / 2, 0, 100, 100, WHITE, &alarm_texture);
    clock_data.snooze_button = create_button(get_current_width() / 2, (float) (get_current_height() / 1.5), 600, 50, DARKBLUE, NULL);
    clock_data.toggle_button = create_button(0, (float) (get_current_height() / 1.5), 100, 100, RED, &toggle_texture);
}