#include <stdlib.h>

#include "rain.h"
#include "../utils.h"

#define DEFAULT_RAIN_PROB 50
#define DEFAULT_RAIN_MAX_LENGTH 75
#define DEFAULT_RAIN_MIN_LENGTH 45
#define DEFAULT_RAIN_MAX_LOOPS 9
#define DEFAULT_RAIN_MIN_LOOPS 4

#define LIGHTNING_PROB 50
#define LIGHTNING_MAX_LENGTH 106
#define LIGHTNING_MIN_LENGTH 90
#define LIGHTNING_MAX_LOOPS 4
#define LIGHTNING_MIN_LOOPS 2

static void update_rain_amb(AmbiancePtr amb)
{
    int r = rand() % 100;

    if (r > DEFAULT_RAIN_PROB)
    {
        amb->min_length = DEFAULT_RAIN_MIN_LENGTH;
        amb->max_length = DEFAULT_RAIN_MAX_LENGTH;
        amb->max_loops = rand_range(DEFAULT_RAIN_MIN_LOOPS, DEFAULT_RAIN_MAX_LOOPS);

    } else
    {
        amb->min_length = LIGHTNING_MIN_LENGTH;
        amb->max_length = LIGHTNING_MAX_LENGTH;
        amb->max_loops = rand_range(LIGHTNING_MIN_LOOPS, LIGHTNING_MAX_LOOPS);
    }

}

void switch_to_rain_amb(AmbiancePtr amb)
{
    amb->setup = update_rain_amb;
    music_init(&amb->music_h, "assets/rain.mp3");
    music_set_vol(&amb->music_h, 0.15);

    update_rain_amb(amb);
}