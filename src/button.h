#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include "utils.h"
#include "sprite.h"

struct Button
{
    Sprite* sprite;
    int is_pressed;
};

struct Button create_button(Sprite* sprite);
int check_pressed(struct Button* button);
#endif