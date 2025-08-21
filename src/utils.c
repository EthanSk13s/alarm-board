#include "utils.h"

int compare_color(Color x, Color y)
{
    return x.r == y.r && x.b == y.b && x.g == y.g;
}

Vector2 utils_center_text(Font font, Vector2 origin, char* text, int font_size, int spacing)
{
    Vector2 center_text = MeasureTextEx(font, text, font_size, 2);
    Vector2 center = { ((float) origin.x / 2) - (center_text.x / 2),
                       ((float) origin.y / 2) - (center_text.y / 2)
                     };

    return center;
}