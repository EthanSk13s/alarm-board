#ifndef AMBIANCE_H
#define AMBIANCE_H

#include "../music.h"

typedef struct Ambiance* AmbiancePtr;

typedef void (*AmbianceLoopSetupFunc) (AmbiancePtr);

struct Ambiance
{
    int min_length;
    int max_length;
    int loops;
    int max_loops;
    AmbianceLoopSetupFunc setup;
    MusicHandler music_h;
};

void amb_default_init(AmbiancePtr amb);

#endif