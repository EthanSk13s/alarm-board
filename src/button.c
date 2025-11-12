#include <raylib.h>
#include <stdlib.h>

#include "button.h"
#include "info_storage.h"
#include "sprite.h"

int btn_init(Button* btn, Sprite* sprite, void (*btn_callback) (void*), void* passed_data)
{
    if (btn == NULL)
    {
        return -1;
    }

    btn->is_pressed = false;
    btn->pressed_fn = btn_callback;
    btn->passed_data = passed_data;
    if (sprite == NULL)
    {
        btn->sprite = malloc(sizeof(Sprite));
        btn->sprite->texture = NULL;
        btn->sprite->color = PURPLE;
    } else
    {
        btn->sprite = sprite;
    }

    return 0;
}

int check_pressed(Button* button)
{
    Vector2 mouse_point = get_virt_mouse();

    if (CheckCollisionPointRec(mouse_point, button->sprite->bounds) && button->sprite->visible)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsGestureDetected(GESTURE_TAP))
        {
            if (button->pressed_fn != NULL)
            {
                button->pressed_fn(button->passed_data);
            }
        }

    }

    return CheckCollisionPointRec(mouse_point, button->sprite->bounds) && button->sprite->visible;
}