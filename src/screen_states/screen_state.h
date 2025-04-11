#ifndef SCREEN_STATE_H
#define SCREEN_STATE_H

typedef struct ScreenState* ScreenStatePtr;

typedef void (*EventDrawFunc) (ScreenStatePtr);
typedef void (*EventUpdateFunc) (ScreenStatePtr);

struct ScreenState
{
    EventDrawFunc draw;
    EventUpdateFunc update;
};

void default_state(ScreenStatePtr state);

#endif