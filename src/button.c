#include <raylib.h>

#include "button.h"
#include "info_storage.h"

struct Button create_button(int pos_x, int pos_y, int width, int height, Color color)
{
    struct Button btn = { };
    btn.is_pressed = false;

    Rectangle btn_bounds = {pos_x, pos_y, width, height};
    btn.bounds = btn_bounds;
    btn.color = color;

    return btn;
}

void update_button_pos_x(struct Button* button, int pos_x)
{
    button->bounds.x = pos_x;
}

void draw_button(struct Button* button)
{
    DrawRectangleRec(button->bounds, button->color);
}

int check_pressed(struct Button* button)
{
    Vector2 mouse_point = get_virt_mouse();

    return CheckCollisionPointRec(mouse_point, button->bounds);
}