#include <stdlib.h>
#include <raylib.h>
#include <pigpio.h>
#include <pthread.h>
#include <time.h>

#include "alarm.h"
#include "cfg.h"
#include "ds/hash_table.h"
#include "renderer.h"
#include "info_storage.h"
#include "managers/screen_man.h"
#include "managers/texture_man.h"
#include "brightness/sensor/VEML7700_pigpio.h"
#include "brightness/brightness_ctrl.h"


#define CFG_LOCATION "./config"

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
    int result = gpioInitialise();

    if (load_alarm() == 0)
    {
        return 1;
    }

    pthread_t brightness_thread;
    if (result >= 0)
    {
        result = VEML7700_setup();
        if (result > -1)
        {
            pthread_create(&brightness_thread, NULL, (void*) brightness_ctrl_thread, NULL);
        }
    }

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Renderer* renderer = malloc(sizeof(Renderer));
    TextureManager* texture_man = texture_manager_create();
    texture_manager_add(texture_man, "alarm", "assets/alarm.png");
    texture_manager_add(texture_man, "set-alarm", "assets/set-alarm.png");
    texture_manager_add(texture_man, "alarm-arrow", "assets/alarm-arrow.png");
    texture_manager_add(texture_man, "snooze", "assets/snooze.png");
    texture_manager_add(texture_man, "clock", "assets/clock.png");

    init_renderer(renderer, screenWidth, screenHeight);
    init_storage(renderer, texture_man);

    ScreenManager* screen_man = init_screen_man();
    if (screen_man == NULL)
    {
        return 1;
    }

    ht* cfg_map = ht_create();
    int parse_result = cfg_parse(cfg_map, CFG_LOCATION);
    if (parse_result == -1)
    {
        return 1;
    }

    parse_result = cfg_load(cfg_map);
    if (parse_result == -1)
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
        //printf("%f\n", VEML7700_read_lux(1));

        screen_man->state.update(&screen_man->state);

        draw_to_target(renderer, &screen_man->state);
        render_target(renderer);
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free_renderer(renderer);
    free_texture_manager(texture_man);

    if (result >= 0)
    {
        brightness_kill_thread();
        pthread_join(brightness_thread, NULL);
        VEML7700_close();
    }

    cfg_save(CFG_LOCATION);

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}