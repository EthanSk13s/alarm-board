#include <raylib.h>
#include <time.h>

#include "info_storage.h"
#include "renderer.h"
struct InfoStorage storage;

void init_storage(Renderer* renderer, TextureManager* texture_man)
{
    storage.texture_man = texture_man;
    storage.renderer = renderer;

    storage.alarm_time.tm_hour = 0;
    storage.alarm_time.tm_min = 0;
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