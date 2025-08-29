#ifndef UTILS_H
#define UTILS_H

#include <raylib.h>
#include <stddef.h>

typedef enum
{
    KELVIN,
    IMPERIAL,
    METRIC
} Units;

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
char* parse_wthr_icon(char* icon_id);
char utils_get_temp_unit_symbol(Units unit);
void draw_text_with_rect_bound(Font font, const char* text, Rectangle rec, float font_size, float spacing, int word_wrap, Color tint);

#endif