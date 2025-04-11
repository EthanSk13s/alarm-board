#include <stdio.h>
#include <stdlib.h>
#include "alarm.h"
#include "renderer.h"
#include "screen_man.h"
#include "raylib.h"

#include "info_storage.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1280;
    int screenHeight = 720;
    int font_size = 200;
    const int flags = FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT;

    InitWindow(screenWidth, screenHeight, "rayalarm");
    SetWindowState(flags);
    InitAudioDevice();              // Initialize audio device

    if (load_alarm() == 0)
    {
        return 1;
    }

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Renderer* renderer = malloc(sizeof(Renderer));
    init_renderer(renderer, screenWidth, screenHeight);
    init_storage(renderer);

    ScreenManager* screen_man = init_screen_man();
    if (screen_man == NULL)
    {
        return 1;
    }

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (is_alarm_playing())
        {
            update_stream();
        }
        update_virt_mouse(renderer);
        if (IsWindowResized())
        {
            update_game_screen(renderer);
        }

        screen_man->state.update(&screen_man->state);

        draw_to_target(renderer, &screen_man->state);
        render_target(renderer);
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free_renderer(renderer);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}