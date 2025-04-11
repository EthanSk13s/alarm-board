#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#include "renderer.h"

void init_renderer(Renderer* renderer, int game_width, int game_height)
{
    renderer->game_width = game_width;
    renderer->game_height = game_height;
    renderer->target = LoadRenderTexture(game_width, game_height);
    renderer->virt_mouse.x = 0;
    renderer->virt_mouse.y = 0;
    SetTextureFilter(renderer->target.texture, TEXTURE_FILTER_ANISOTROPIC_16X);

    update_game_screen(renderer);
}

void free_renderer(Renderer *renderer)
{
    UnloadRenderTexture(renderer->target);
    free(renderer);
}

void draw_to_target(Renderer* renderer, ScreenStatePtr state)
{
    BeginTextureMode(renderer->target);
        state->draw(state);
    EndTextureMode();
}

void render_target(Renderer* renderer)
{
    int width = renderer->game_width;
    int height = renderer->game_height;
    float scale = renderer->scale;
    Texture2D texture = renderer->target.texture;

    BeginDrawing();
        DrawTexturePro(renderer->target.texture,
                       (Rectangle) { 0.0f, 0.0f, (float) texture.width, (float) -texture.height },
                       (Rectangle) { (GetScreenWidth() - ((float) width * scale)) * 0.5f,
                                     (GetScreenHeight() - ((float) height * scale)) * 0.5f,
                                     (float) width * scale,
                                     (float) height * scale },
                       (Vector2){ 0, 0 },
                       0.0f,
                       WHITE);
    EndDrawing();
}

void update_game_screen(Renderer* renderer)
{
    renderer->scale = MIN((float) GetScreenWidth() / renderer->game_width,
                          (float) GetScreenHeight() / renderer->game_height);
}

void update_virt_mouse(Renderer* renderer)
{
    Vector2 mouse = GetMousePosition();

    int width = renderer->game_width;
    int height = renderer->game_height;
    float scale = renderer->scale;
    renderer->virt_mouse.x = (mouse.x - (GetScreenWidth() - (width * scale)) * 0.5f) / scale;
    renderer->virt_mouse.y = (mouse.y - (GetScreenHeight() - (height * scale)) * 0.5f) / scale;

    renderer->virt_mouse = Vector2Clamp(renderer->virt_mouse,
                                        (Vector2) { 0, 0 },
                                        (Vector2) { (float) width, (float) height });
}