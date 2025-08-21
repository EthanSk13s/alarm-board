#ifndef BUTTON_H
#define BUTTON_H

#include "sprite.h"

typedef struct
{
    int is_pressed;
    Sprite* sprite;
    void* passed_data;
    void (*pressed_fn) (void*);
}  Button;

int btn_init(Button* btn, Sprite* sprite, void (*btn_callback) (void*), void* passed_data);
int check_pressed(Button* button);
#endif