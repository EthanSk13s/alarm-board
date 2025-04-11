#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

struct Button
{
    int is_pressed;
    Rectangle bounds;
    Color color;
};

struct Button create_button(int pos_x, int pos_y, int width, int height, Color color);
void draw_button(struct Button* button);
int check_pressed(struct Button* button);
#endif