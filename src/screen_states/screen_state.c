#include "screen_state.h"

static void default_draw(ScreenStatePtr state)
{

}

static void default_update(ScreenStatePtr state)
{

}

void default_state(ScreenStatePtr state)
{
    state->draw = default_draw;
    state->update = default_update;
}