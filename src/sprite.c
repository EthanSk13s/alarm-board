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
    Rectangle dest = sprite->bounds;
    Vector2 origin = {0, 0};
    if (rotation != 0)
    {
        origin.x = sprite->bounds.width / 2.f;
        origin.y = sprite->bounds.height / 2.f;

        dest.x += origin.x;
        dest.y += origin.y;
    }
    Rectangle src = { 0, 0, sprite->bounds.width / scale, sprite->bounds.height / scale};

    DrawTexturePro(*sprite->texture, src, dest, origin, rotation, sprite->color);
}
