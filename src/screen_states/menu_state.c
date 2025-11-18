#include "menu_state.h"

#include "../info_storage.h"
#include "../managers/sprite_man.h"
#include "../managers/ui_man.h"
#include "../widgets/template_buttons.h"

struct MenuData
{
    int textures_loaded;
    SpriteManager* sprite_manager;
    UIManager ui_manager;
};

static struct MenuData menu_data;

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
        TextureManager* txt_man = get_texture_man();
        menu_data.sprite_manager = create_sprite_manager();
        ui_man_init(&menu_data.ui_manager);

        // TODO: Create this button bar separately. 80 is from total bar width from
        // combining textures.
        int btn_bar_x = (width / 2) - (480 / 2);
        int btn_bar_y = (height / 2) - 80;

        button_clock(txt_man,
                     menu_data.sprite_manager,
                     &menu_data.ui_manager,
                     state,
                     btn_bar_x,
                     btn_bar_y);
        button_forecasts(txt_man,
                         menu_data.sprite_manager,
                         &menu_data.ui_manager,
                         state,
                         btn_bar_x + 160,
                         btn_bar_y);
        button_ambiance(txt_man,
                         menu_data.sprite_manager,
                         &menu_data.ui_manager,
                         state,
                         btn_bar_x + 320,
                         btn_bar_y);

        menu_data.textures_loaded = 1;
    }
}
