#include <stdio.h>

#include "ui_man.h"

int ui_man_init(UIManager* ui_manager)
{
    if (ui_manager == NULL)
    {
        return -1;
    }

    for (int i = 0; i < UI_MAX_SIZE; i++)
    {
        ui_manager->btns[i] = NULL;
    }

    return 0;
}

int ui_man_poll(UIManager* ui_manager)
{
    if (ui_manager == NULL)
    {
        return -1;
    }

    for (int i = 0; i < UI_MAX_SIZE; i++)
    {
        Button* curr_btn = ui_manager->btns[i];
        if (curr_btn != NULL)
        {
            check_pressed(curr_btn);
        }
    }

    return 0;
}

int ui_man_add(UIManager* ui_manager, Button *btn)
{
    if (ui_manager == NULL)
    {
        return -1;
    }

    int btn_id = -1;
    for (int i = 0; i < UI_MAX_SIZE; i++)
    {
        if (ui_manager->btns[i] == NULL)
        {
            btn_id = i;
            ui_manager->btns[i] = btn;
            break;
        }
    }

    return btn_id;
}

int ui_man_remove(UIManager* ui_manager, int btn_id)
{
    if (ui_manager == NULL)
    {
        return -1;
    }

    if (btn_id >= UI_MAX_SIZE)
    {
        return -1;
    }

    ui_manager->btns[btn_id] = NULL;

    return 0;
}

Button* ui_man_pop(UIManager* ui_manager)
{
    if (ui_manager == NULL)
    {
        return NULL;
    }

    Button* pop_btn = NULL;
    for (int i = UI_MAX_SIZE - 1; i >= 0; i--)
    {
        if (ui_manager->btns[i] != NULL)
        {
            pop_btn = ui_manager->btns[i];
            ui_manager->btns[i] = NULL;

            break;
        }
    }

    return pop_btn;
}