#ifndef TEXTURE_MAN_H
#define TEXTURE_MAN_H
#include <raylib.h>

#include "ds/hash_table.h"

typedef struct TextureManager TextureManager;

TextureManager* texture_manager_create();
int texture_manager_add(TextureManager* texture_manager, char* key, char* filepath);
void free_texture_manager(TextureManager* texture_manager);
Texture2D* texture_manager_get(TextureManager* texture_manager, char* key);

#endif