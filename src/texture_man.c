#include <raylib.h>
#include <stdlib.h>

#include "texture_man.h"
#include "ds/hash_table.h"

struct TextureManager
{
    ht* texture_maps;
};

typedef struct
{
    Texture2D texture;
} TextureContainer;

TextureManager* texture_manager_create()
{
    TextureManager* texture_man = malloc(sizeof(TextureManager));
    if (texture_man == NULL)
    {
        return NULL;
    }

    texture_man->texture_maps = ht_create();
    if (texture_man->texture_maps == NULL)
    {
        free(texture_man);
        return NULL;
    }

    return texture_man;
}

int texture_manager_add(TextureManager* texture_manager, char* key, char* filepath)
{
    if (texture_manager == NULL)
    {
        return -1;
    }

    Image image = LoadImage(filepath);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    if (!IsTextureValid(texture))
    {
        UnloadTexture(texture);
        return -1;
    }

    TextureContainer* texture_container = malloc(sizeof(TextureContainer));
    texture_container->texture = texture;

    const char* result = ht_set(texture_manager->texture_maps, key, texture_container);
    if (result == NULL)
    {
        return -1;
    }

    return 0;
}

Texture2D* texture_manager_get(TextureManager* texture_manager, char* key)
{
    if (texture_manager == NULL)
    {
        return NULL;
    }

    TextureContainer* container = (TextureContainer*) ht_get(texture_manager->texture_maps, key);

    return &container->texture;
}

void free_texture_manager(TextureManager* texture_manager)
{
    hti iter = ht_iterator(texture_manager->texture_maps);
    int result = ht_next(&iter);
    while(result)
    {
        TextureContainer* value = (TextureContainer*) iter.value;
        UnloadTexture(value->texture);
        free(value);

        result = ht_next(&iter);
    }

    ht_destroy(texture_manager->texture_maps);
    free(texture_manager);
}