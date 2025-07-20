#include <stdlib.h>
#include <stdio.h>
#include "sprite_man.h"

#define INITIAL_CAPACITY 8

SpriteManager* create_sprite_manager()
{
    SpriteManager* sprite_manager = malloc(sizeof(SpriteManager));
    if (sprite_manager == NULL)
    {
        return NULL;
    }
    sprite_manager->sprites = malloc(sizeof(Sprite*) * INITIAL_CAPACITY);
    sprite_manager->count = 0;
    sprite_manager->capacity = INITIAL_CAPACITY;

    return sprite_manager;
}

int add_to_sprite_manager(SpriteManager* sprite_manager, Sprite* sprite)
{
    if (sprite_manager == NULL || sprite == NULL)
    {
        return -1;
    }

    if (sprite_manager->count + 1 >= sprite_manager->capacity)
    {
        int new_capacity = sprite_manager->capacity * 2;
        Sprite** new_sprites = realloc(sprite_manager->sprites, sizeof(Sprite*) * new_capacity);
        if (new_sprites == NULL)
        {
            return -1;
        }
        sprite_manager->sprites = new_sprites;
        sprite_manager->capacity = new_capacity;
    }

    sprite_manager->sprites[sprite_manager->count] = sprite;
    sprite_manager->count++;

    return sprite_manager->count - 1;
}

void draw_sprites(SpriteManager* sprite_manager)
{
    for (int i = 0; sprite_manager->count > i; i++)
    {
        if (sprite_manager->sprites[i] != NULL)
        {
            if (sprite_manager->sprites[i]->visible)
            {
                draw_sprite(sprite_manager->sprites[i]);
            }

        }
    }
}

void sprite_man_free(SpriteManager* sprite_manager)
{
    for (int i = 0; sprite_manager->count > i; i++)
    {
        if (sprite_manager->sprites[i] != NULL)
        {
            free(sprite_manager->sprites[i]);
        }
    }

    free(sprite_manager);
}

void toggle_sprite_visibility(SpriteManager* sprite_manager, int id)
{
    if (sprite_manager->capacity <= id)
    {
        return;
    }

    if (sprite_manager->sprites[id] == NULL)
    {
        return;
    }

    if (sprite_manager->sprites[id]->visible)
    {
        sprite_manager->sprites[id]->visible = 0;
    } else
    {
        sprite_manager->sprites[id]->visible = 1;
    }
}