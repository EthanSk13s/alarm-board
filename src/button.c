#include <raylib.h>
#include <stdlib.h>

#include "button.h"
#include "info_storage.h"
#include "sprite.h"

struct Button create_button(Sprite* sprite, void (*btn_callback) (void))
{
    struct Button btn = { };
    btn.is_pressed = false;
    btn.pressed_fn = btn_callback;
    if (sprite == NULL)
    {
        btn.sprite = malloc(sizeof(Sprite));
        btn.sprite->texture = NULL;
        btn.sprite->color = PURPLE;
    } else
    {
        btn.sprite = sprite;
    }

    return btn;
}

int check_pressed(struct Button* button)
{
    Vector2 mouse_point = get_virt_mouse();

    if (CheckCollisionPointRec(mouse_point, button->sprite->bounds) && button->sprite->visible)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsGestureDetected(GESTURE_TAP))
        {
            button->pressed_fn();
        }

    }

    return CheckCollisionPointRec(mouse_point, button->sprite->bounds) && button->sprite->visible;
}