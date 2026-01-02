#include <stdlib.h>

#include "amb_man.h"
#include "../timer.h"
#include "../ambiance/rain.h"

int amb_man_init(AmbianceManager* amb_man)
{
    if (amb_man == NULL)
    {
        return -1;
    }

    amb_man->amb = malloc(sizeof(struct Ambiance));

    amb_man->has_loaded = 0;
    amb_man->amb->max_length = -1;
    amb_man->amb->min_length = -1;
    amb_man->amb->max_loops = -1;
    amb_man->amb->setup = NULL;

    return 0;
}

int amb_man_is_done(AmbianceManager *amb_man)
{
    return timer_done(amb_man->amb_timer);
}

void amb_man_update(AmbianceManager* amb_man)
{
    if (amb_man == NULL)
    {
        return;
    }

    if (!amb_man->has_loaded)
    {
        return;
    }

    if (!music_is_playing(&amb_man->amb->music_h))
    {
        return;
    }

    if (amb_man_is_done(amb_man))
    {
        amb_man_stop(amb_man);
    }

    struct Ambiance* amb = amb_man->amb;
    float curr_pos = music_get_pos(&amb->music_h);
    if (curr_pos >= amb->max_length && amb->max_loops <= amb->loops)
    {
        if (amb->setup != NULL)
        {
            amb->setup(amb);
            amb->loops = 0;
            music_seek(&amb->music_h, amb->min_length);
        }
    }

    if (curr_pos >= amb->max_length && amb->max_loops > amb->loops)
    {
        amb->loops++;
        music_seek(&amb->music_h, amb->min_length);
    }

    music_update(&amb_man->amb->music_h);
}

int amb_man_start(AmbianceManager* amb_man, int length)
{
    if (amb_man == NULL)
    {
        return -1;
    }

    if (music_play(&amb_man->amb->music_h) == -1)
    {
        return -1;
    }

    timer_start(&amb_man->amb_timer, length);

    return 0;
}

void amb_man_stop(AmbianceManager* amb_man)
{
    if (amb_man == NULL)
    {
        return;
    }

    music_stop(&amb_man->amb->music_h);
}

void amb_man_switch_to_rain(AmbianceManager* amb_man)
{
    amb_man->has_loaded = 1;
    if (amb_man == NULL)
    {
        return;
    }

    switch_to_rain_amb(amb_man->amb);
}

void amb_man_free(AmbianceManager* amb_man)
{
    if (amb_man == NULL)
    {
        return;
    }

    free(amb_man->amb);
    amb_man->amb = NULL;

    free(amb_man);
    amb_man = NULL;
}