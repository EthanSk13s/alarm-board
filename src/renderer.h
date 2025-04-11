#ifndef RENDERER_H
#define RENDERER_H

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

#include <raylib.h>
#include "screen_states/screen_state.h"

typedef struct
{
    RenderTexture2D target;
    Vector2 virt_mouse;
    float scale;
    int game_width;
    int game_height;
} Renderer;

void init_renderer(Renderer* renderer, int game_width, int game_height);
void free_renderer(Renderer* renderer);
void draw_to_target(Renderer* renderer, ScreenStatePtr state);
void render_target(Renderer* renderer);
void update_game_screen(Renderer* renderer);
void update_virt_mouse(Renderer* renderer);

#endif