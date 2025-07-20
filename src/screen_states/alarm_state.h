#ifndef ALARM_STATE_H
#define ALARM_STATE_H

#include "screen_state.h"

void transition_to_alarm(ScreenStatePtr state);
void clean_up_alarm_state();

#endif