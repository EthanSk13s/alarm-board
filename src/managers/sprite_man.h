#ifndef SPRITE_MAN_H
#define SPRITE_MAN_H
#include "../sprite.h"

typedef struct
{
    Sprite** sprites;
    int count;
    int capacity;
} SpriteManager;

SpriteManager* create_sprite_manager();
int add_to_sprite_manager(SpriteManager* sprite_manager, Sprite* sprite);
void free_sprite_manager(SpriteManager* sprite_manager);
void draw_sprites(SpriteManager* sprite_manager);
void toggle_sprite_visibility(SpriteManager* sprite_manager, int id);
#endif