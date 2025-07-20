#ifndef UI_MAN_H
#define UI_MAN_H

#include "../button.h"

#define UI_MAX_SIZE 50

typedef struct
{
    Button* btns[UI_MAX_SIZE];
} UIManager;

int ui_man_init(UIManager* ui_manager);
int ui_man_poll(UIManager* ui_manager);
int ui_man_add(UIManager* ui_manager, Button* btn);
int ui_man_remove(UIManager* ui_manager, int btn_id);
Button* ui_man_pop(UIManager* ui_manager);

#endif