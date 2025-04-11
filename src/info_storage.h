#ifndef INFO_STORAGE_H
#define INFO_STORAGE_H

#include <raylib.h>
#include <time.h>
#include "renderer.h"
#define CLOCK_FONT_SIZE 256

struct InfoStorage
{
    Renderer* renderer;
    int height;
    int width;
    struct tm alarm_time;
};

void init_storage(Renderer* renderer);
int get_current_height();
int get_current_width();
Vector2 get_virt_mouse();
struct tm get_alarm();

void set_current_height(int height);
void set_current_width(int width);
void set_alarm_hour(int hour);
void set_alarm_min(int min);

#endif