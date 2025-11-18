#ifndef TEMPLATE_BUTTONS_H
#define TEMPLATE_BUTTONS_H

#include "../managers/sprite_man.h"
#include "../managers/texture_man.h"
#include "../managers/ui_man.h"

int button_forecasts(TextureManager* texture_man, SpriteManager* sprite_man, UIManager* ui_man, void* state, int x, int y);
int button_clock(TextureManager* texture_man, SpriteManager* sprite_man, UIManager* ui_man, void* state, int x, int y);
int button_ambiance(TextureManager* texture_man, SpriteManager* sprite_man, UIManager* ui_man, void* state, int x, int y);
int button_menu(TextureManager* texture_man, SpriteManager* sprite_man, UIManager* ui_man, void* state, int x, int y);
#endif