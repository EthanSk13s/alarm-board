#ifndef SPRITE_H
#define SPRITE_H
#include <raylib.h>

#include "utils.h"

typedef struct
{
    Rectangle bounds;
    Texture2D* texture;
    TextureSet texture_opts;
    Color color;
} Sprite;

Sprite* create_sprite(float x, float y, Texture2D* texture, TextureSet texture_opts, Color color);
void free_sprite(Sprite* sprite);
void draw_sprite(Sprite* sprite);

#endif