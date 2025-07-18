#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include "utils.h"
#include "sprite.h"

typedef struct
{
    int is_pressed;
    Sprite* sprite;
    void (*pressed_fn) (void);
}  Button;

int btn_init(Button* btn, Sprite* sprite, void (*btn_callback) (void));
int check_pressed(Button* button);
#endif