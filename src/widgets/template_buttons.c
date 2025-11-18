#include <stdlib.h>

#include "template_buttons.h"

#include "../utils.h"
#include "../screen_states/ambiance_state.h"
#include "../screen_states/clock_state.h"
#include "../screen_states/menu_state.h"
#include "../screen_states/weather_state.h"

#define BTN_TEXTURE_SET (TextureSet) { 10, 0 }

static void wthr_btn_callback(void* state)
{
    transition_to_wthr_state(state);
}

static void amb_btn_callback(void* state)
{
    transition_to_amb_state(state);
}

static void clock_btn_callback(void* state)
{
    transition_to_clock(state);
}

static void menu_btn_callback(void* state)
{
    transition_to_menu(state);
}

int button_forecasts(TextureManager* texture_man, SpriteManager* sprite_man, UIManager* ui_man, void* state, int x, int y)
{
    Texture2D* wthr_texture = texture_manager_get(texture_man, "day_clouded");
    Sprite* wthr_sprite = create_sprite(x, y, wthr_texture, BTN_TEXTURE_SET, WHITE, 0);
    
    int sprite_id = add_to_sprite_manager(sprite_man, wthr_sprite);

    Button* wthr_btn = malloc(sizeof(Button));
    btn_init(wthr_btn, wthr_sprite, wthr_btn_callback, state);

    ui_man_add(ui_man, wthr_btn);

    return sprite_id;
}

int button_clock(TextureManager* texture_man, SpriteManager* sprite_man, UIManager* ui_man, void* state, int x, int y)
{
    Texture2D* clock_texture = texture_manager_get(texture_man, "clock");
    Sprite* clock_sprite = create_sprite(x, y, clock_texture, BTN_TEXTURE_SET, WHITE, 0);
    
    int sprite_id = add_to_sprite_manager(sprite_man, clock_sprite);

    Button* clock_btn = malloc(sizeof(Button));
    btn_init(clock_btn, clock_sprite, clock_btn_callback, state);

    ui_man_add(ui_man, clock_btn);

    return sprite_id;
}

int button_ambiance(TextureManager* texture_man, SpriteManager* sprite_man, UIManager* ui_man, void* state, int x, int y)
{
    Texture2D* amb_texture = texture_manager_get(texture_man, "mist");
    Sprite* amb_sprite = create_sprite(x, y, amb_texture, (TextureSet) { 10, 90 }, WHITE, 0);
    
    int sprite_id = add_to_sprite_manager(sprite_man, amb_sprite);

    Button* amb_btn = malloc(sizeof(Button));
    btn_init(amb_btn, amb_sprite, amb_btn_callback, state);

    ui_man_add(ui_man, amb_btn);

    return sprite_id;
}

int button_menu(TextureManager* texture_man, SpriteManager* sprite_man, UIManager* ui_man, void* state, int x, int y)
{
    Texture2D* menu_texture = texture_manager_get(texture_man, "hamburger");
    Sprite* menu_sprite = create_sprite(x, y, menu_texture, BTN_TEXTURE_SET, WHITE, 0);
    
    int sprite_id = add_to_sprite_manager(sprite_man, menu_sprite);

    Button* menu_btn = malloc(sizeof(Button));
    btn_init(menu_btn, menu_sprite, menu_btn_callback, state);

    ui_man_add(ui_man, menu_btn);

    return sprite_id;
}