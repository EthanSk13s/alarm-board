#ifndef UTILS_H
#define UTILS_H

#include <raylib.h>
#include <stddef.h>

typedef struct
{
    float scale;
    int rotation;
}  TextureSet;

typedef struct
{
    char* buffer;
    size_t buf_length;
    size_t max_length;
} Buffer;

int compare_color(Color x, Color y);
Vector2 utils_center_text(Font font, Vector2 origin, char* text, int font_size, int spacing);

#endif