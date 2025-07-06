#include <raylib.h>
#include <stdlib.h>

#include "sprite.h"
#include "ds/hash_table.h"

Sprite* create_sprite(float x, float y, Texture2D* texture, TextureSet texture_opts, Color color)
{
    Sprite* sprite = malloc(sizeof(Sprite));
    if (sprite == NULL || texture == NULL)
    {
        return NULL;
    }

    float width = texture->width * texture_opts.scale;
    float height = texture->height * texture_opts.scale;
    Rectangle bounds = {x, y, width, height};

    sprite->bounds = bounds;
    sprite->texture = texture;
    sprite->texture_opts = texture_opts;
    sprite->color = color;

    return sprite;
}

void free_sprite(Sprite *sprite)
{
    if (sprite == NULL)
    {
        return;
    }
    free(sprite);
}

void draw_sprite(Sprite* sprite)
{
    if (sprite == NULL || sprite->texture == NULL)
    {
        return;
    }

    float scale = sprite->texture_opts.scale;
    float rotation = sprite->texture_opts.rotation;
    Vector2 position = {sprite->bounds.x, sprite->bounds.y};

    DrawTextureEx(*sprite->texture, position, rotation, scale, sprite->color);
}
