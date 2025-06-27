#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include "utils.h"

struct Button
{
    int is_pressed;
    Rectangle bounds;
    Color color;
    Texture texture;
};

struct Button create_button(int pos_x, int pos_y, int width, int height, Color color, TextureSet* opt_texture);
void draw_button(struct Button* button);
int check_pressed(struct Button* button);
#endif