#ifndef INFO_STORAGE_H
#define INFO_STORAGE_H

#include <raylib.h>
#include <time.h>
#include "renderer.h"
#include "managers/texture_man.h"

#define CLOCK_FONT_SIZE 256

struct InfoStorage
{
    Renderer* renderer;
    TextureManager* texture_man;
    int height;
    int width;
    struct tm alarm_time;
};

void init_storage(Renderer* renderer, TextureManager* texture_man);
int get_current_height();
int get_current_width();
Vector2 get_virt_mouse();
struct tm get_alarm();
TextureManager* get_texture_man();

void set_current_height(int height);
void set_current_width(int width);
void set_alarm_hour(int hour);
void set_alarm_min(int min);

#endif