#include <raylib.h>
#include <stdlib.h>

#include "utils.h"
#include "weather/forecasts.h"

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

char* parse_wthr_icon(char* icon_id)
{
    int id = atoi(icon_id);
    char time = icon_id[FORECAST_MAX_ICON_LENGTH - 2];

    char* key;
    switch(id)
    {
        case 1:
        {
            if (time == 'd')
            {
                key = "sun";
            } else
            {
                key = "moon";
            }
        } break;
        case 2:
        {
            if (time == 'd')
            {
                key = "day_clouded";
            } else
            {
                key = "night_clouded";
            }
        } break;
        case 3: key = "scattered"; break;
        case 4: key = "broken"; break;
        case 9: key = "shower"; break;
        case 10:
        {
            if (time == 'd')
            {
                key = "day_rain";
            } else
            {
                key = "night_rain";
            }
        } break;
        case 11: key = "rainstorm"; break;
        case 13: key = "snow"; break;
        case 50: key = "mist"; break;
        default: key = "unknown";
    }

    return key;
}

// Implementatiom is mostly from:
// https://www.raylib.com/examples/text/loader.html?name=text_rectangle_bounds
void draw_text_with_rect_bound(Font font, const char* text, Rectangle rec, float font_size, float spacing, int word_wrap, Color tint)
{
    int length = TextLength(text);

    float text_offset_y = 0;
    float text_offset_x = 0.0f;

    float scale_factor = font_size / (float) font.baseSize;

    enum { MEASURE_STATE = 0, DRAW_STATE = 1 };
    int state = word_wrap ? MEASURE_STATE : DRAW_STATE;

    int start_line = -1;
    int end_line = -1;
    int last_k = -1;

    for (int i = 0, k = 0; i < length; i++, k++)
    {
        int codepoint_byte_count = 0;
        int codepoint = GetCodepoint(&text[i], &codepoint_byte_count);
        int index = GetGlyphIndex(font, codepoint);

        if (codepoint == 0x3f)
        {
            codepoint_byte_count = 1;
        }
        i += codepoint_byte_count - 1;

        float glyph_width = 0;
        if (codepoint != '\n')
        {
            if (font.glyphs[index].advanceX == 0)
            {
                glyph_width = font.recs[index].width * scale_factor;
            } else
            {
                glyph_width = font.glyphs[index].advanceX * scale_factor;
            }

            if (i + 1 < length)
            {
                glyph_width += spacing;
            }
        }

        if (state == MEASURE_STATE)
        {
            if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n'))
            {
                end_line = i;
            }

            if ((text_offset_x + glyph_width) > rec.width)
            {
                end_line = (end_line < 1) ? i : end_line;
                if (i == end_line)
                {
                    end_line -= codepoint_byte_count;
                }

                if ((start_line + codepoint_byte_count) == end_line)
                {
                    end_line = i - codepoint_byte_count;
                }

                state = !state;
            } else if ((i + 1) == length)
            {
                end_line = i;
                state = !state;
            } else if (codepoint == '\n')
            {
                state = !state;
            }

            if (state == DRAW_STATE)
            {
                text_offset_x = 0;
                i = start_line;
                glyph_width = 0;

                int tmp = last_k;
                last_k = k - 1;
                k = tmp;
            }
        } else
        {
            if (codepoint == '\n')
            {
                if (!word_wrap)
                {
                    text_offset_y += (font.baseSize + font.baseSize / 2) * scale_factor;
                    text_offset_x = 0;
                }
            } else
            {
                if (!word_wrap && ((text_offset_x + glyph_width) > rec.width))
                {
                    text_offset_y += (font.baseSize + font.baseSize / 2) * scale_factor;
                    text_offset_x = 0;
                }

                // When the text overflows the rectangle height, stop drawing.
                if ((text_offset_y + font.baseSize * scale_factor) > rec.height) break;

                if ((codepoint != ' ') && (codepoint != '\t'))
                {
                    DrawTextCodepoint(font,
                                      codepoint,
                            (Vector2) { rec.x + text_offset_x, rec.y + text_offset_y },
                            font_size,
                                      tint);
                }
            }

            if (word_wrap && (i == end_line))
            {
                text_offset_y += (font.baseSize + font.baseSize / 2) * scale_factor;
                text_offset_x = 0;
                start_line = end_line;
                end_line = -1;
                glyph_width = 0;
                k = last_k;

                state = !state;
            }
        }

        if ((text_offset_x != 0) || (codepoint != ' ')) text_offset_x += glyph_width;
    }
}