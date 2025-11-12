#include <stdlib.h>

#include "menu_state.h"
#include "ambiance_state.h"
#include "clock_state.h"
#include "weather_state.h"

#include "../info_storage.h"
#include "../managers/sprite_man.h"
#include "../managers/ui_man.h"

struct MenuData
{
    int textures_loaded;
    SpriteManager* sprite_manager;
    UIManager ui_manager;
};

static struct MenuData menu_data;
static void clock_btn_callback(void* state);
static void wthr_btn_callback(void* state);
static void amb_btn_callback(void* state);

void menu_update(ScreenStatePtr state)
{
    ui_man_poll(&menu_data.ui_manager);
}

void menu_draw(ScreenStatePtr state)
{
    ClearBackground(BLACK);
    draw_sprites(menu_data.sprite_manager);
}

void transition_to_menu(ScreenStatePtr state)
{
    default_state(state);

    state->update = menu_update;
    state->draw = menu_draw;

    int width = get_current_width();
    int height = get_current_height();

    if (!menu_data.textures_loaded)
    {
        menu_data.sprite_manager = create_sprite_manager();
        ui_man_init(&menu_data.ui_manager);

        Texture2D* wthr_texture = texture_manager_get(get_texture_man(), "day_clouded");
        Texture2D* clock_texture = texture_manager_get(get_texture_man(), "clock");
        Texture2D* amb_texture = texture_manager_get(get_texture_man(), "mist");

        TextureSet btn_texture_opts = { 10 , 0 };
        TextureSet amb_texture_opts = { 10 , 90 };
        // TODO: Create this button bar separately. 80 is from total bar width from
        // combining textures.
        int btn_bar_x = (width / 2) - (480 / 2);
        int btn_bar_y = height / 2; 
        Sprite* clock_sprite = create_sprite(btn_bar_x,
                                             btn_bar_y - 80,
                                             clock_texture,
                                             btn_texture_opts,
                                             WHITE,
                                             0);
        Sprite* wthr_sprite = create_sprite(btn_bar_x + 160,
                                            btn_bar_y - 80,
                                            wthr_texture,
                                            btn_texture_opts,
                                            WHITE,
                                            0);
        Sprite* amb_sprite = create_sprite(btn_bar_x + 320,
                                           btn_bar_y - 80,
                                           amb_texture,
                                           amb_texture_opts,
                                           WHITE,
                                           0);

        add_to_sprite_manager(menu_data.sprite_manager, wthr_sprite);
        add_to_sprite_manager(menu_data.sprite_manager, clock_sprite);
        add_to_sprite_manager(menu_data.sprite_manager, amb_sprite);


        Button* clk_btn = malloc(sizeof(Button));
        Button* wthr_btn = malloc(sizeof(Button));
        Button* amb_btn = malloc(sizeof(Button));

        btn_init(clk_btn, clock_sprite, clock_btn_callback, state);
        btn_init(wthr_btn, wthr_sprite, wthr_btn_callback, state);
        btn_init(amb_btn, amb_sprite, amb_btn_callback, state);

        ui_man_add(&menu_data.ui_manager, clk_btn);
        ui_man_add(&menu_data.ui_manager, wthr_btn);
        ui_man_add(&menu_data.ui_manager, amb_btn);

        menu_data.textures_loaded = 1;
    }
}

static void clock_btn_callback(void* state)
{
    transition_to_clock(state);
}

static void wthr_btn_callback(void* state)
{
    transition_to_wthr_state(state);
}

static void amb_btn_callback(void* state)
{
    transition_to_amb_state(state);
}