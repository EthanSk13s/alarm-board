#include <raylib.h>
#include <stdlib.h>

#include "button.h"
#include "info_storage.h"
#include "sprite.h"

struct Button create_button(Sprite* sprite)
{
    struct Button btn = { };
    btn.is_pressed = false;
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

    return CheckCollisionPointRec(mouse_point, button->sprite->bounds);
}