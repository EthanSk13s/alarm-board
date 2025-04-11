#include <time.h>
#include <raylib.h>

#include "alarm_state.h"
#include "clock_state.h"
#include "screen_state.h"
#include "../info_storage.h"
#include "../button.h"

struct AlarmData
{
    char alarm[6];
    struct tm alarm_tm;
    struct Button hour_button_dec;
    struct Button hour_button_inc;
    struct Button minute_button_inc;
    struct Button minute_button_dec;
};

static struct AlarmData alarm_data;

static void update_alarm()
{
    alarm_data.alarm_tm = get_alarm();
    strftime(alarm_data.alarm, 6, "%H:%M", &alarm_data.alarm_tm);
}

static void alarm_update(ScreenStatePtr state)
{
    if (IsKeyPressed(KEY_A))
    {
        transition_to_clock(state);
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
            set_alarm_min(dec);
            update_alarm();
        }
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
    draw_button(&alarm_data.hour_button_dec);
    draw_button(&alarm_data.hour_button_inc);
    draw_button(&alarm_data.minute_button_inc);
    draw_button(&alarm_data.minute_button_dec);
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

    alarm_data.hour_button_dec = create_button((float) width / 2 - (center.x / 3.25),
                                               (float) height / 2 + (center.y / 2),
                                               50, 50, BLUE);
    alarm_data.hour_button_inc = create_button((float) width / 2 - (center.x / 3.25),
                                               (float) height / 2 - (center.y / 1.25),
                                               50, 50, BLUE);

    alarm_data.minute_button_dec = create_button((float) width / 2 + (center.x / 3.25),
                                                 (float) height / 2 + (center.y / 2),
                                                 50, 50, BLUE);
    alarm_data.minute_button_inc = create_button((float) width / 2 + (center.x / 3.25),
                                                 (float) height / 2 - (center.y / 1.25),
                                                 50, 50, BLUE);
}