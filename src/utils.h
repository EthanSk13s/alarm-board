#ifndef UTILS_H
#define UTILS_H

#include <raylib.h>

typedef struct
{
    Texture2D* texture;
    float scale;
}  TextureSet;

int compare_color(Color x, Color y);

#endif