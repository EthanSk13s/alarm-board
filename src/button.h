#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include "utils.h"
#include "sprite.h"

struct Button
{
    int is_pressed;
    Sprite* sprite;
    void (*pressed_fn) (void);
};

struct Button create_button(Sprite* sprite, void (*btn_callback) (void));
int check_pressed(struct Button* button);
#endif