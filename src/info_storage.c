#include <raylib.h>
#include <time.h>
#include <stdio.h>

#include "info_storage.h"
#include "managers/amb_man.h"
#include "renderer.h"

struct InfoStorage storage;

void init_storage(Renderer* renderer, TextureManager* texture_man, AmbianceManager* amb_man)
{
    storage.texture_man = texture_man;
    storage.renderer = renderer;
    storage.amb_man = amb_man;

    storage.alarm_time.tm_hour = 0;
    storage.alarm_time.tm_min = 0;

    int res = music_init(&storage.alarm, "assets/alarm.mp3");
    if (res == -1)
    {
        printf("ERROR: Alarm not found.\n");
    }

    music_set_vol(&storage.alarm, 0.25);
}

MusicHandler get_alarm_music()
{
    return storage.alarm;
}

AmbianceManager* get_amb_man()
{
    return storage.amb_man;
}

int get_current_height()
{
    return storage.renderer->game_height;
}

int get_current_width()
{
    return storage.renderer->game_width;
}

Vector2 get_virt_mouse()
{
    return storage.renderer->virt_mouse;
}

struct tm get_alarm()
{
    return storage.alarm_time;
}

TextureManager* get_texture_man()
{
    return storage.texture_man;
}

void set_alarm_hour(int hour)
{
    if (hour > 23 || hour < 0)
    {
        storage.alarm_time.tm_hour = 0;
    } else
    {
        storage.alarm_time.tm_hour = hour;
    }
}

void set_alarm_min(int min)
{
    if (min > 59 || min < 0)
    {
        storage.alarm_time.tm_min = 0;
    } else
    {
        storage.alarm_time.tm_min = min;
    }
}

void set_current_height(int height)
{
    storage.height = height;
}

void set_current_width(int width)
{
    storage.width = width;
}