#ifndef AMB_MAN_H
#define AMB_MAN_H

#include "../timer.h"
#include "../ambiance/ambiance.h"

typedef struct
{
    Timer amb_timer;
    struct Ambiance* amb;
} AmbianceManager;

int amb_man_init(AmbianceManager* amb_man);
int amb_man_is_done(AmbianceManager* amb_man);
void amb_man_update(AmbianceManager* amb_man);
void amb_man_start(AmbianceManager* amb_man, int length);
void amb_man_stop(AmbianceManager* amb_man);
void amb_man_free(AmbianceManager* amb_man);

void amb_man_switch_to_rain(AmbianceManager* amb_man);


#endif