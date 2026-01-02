#ifndef MUSIC_H
#define MUSIC_H

#include <raylib.h>

typedef struct
{
    float volume;
    Music music;
} MusicHandler;

int music_init(MusicHandler* music_h, char* filepath);
void music_update(MusicHandler* music_h);
void music_set_vol(MusicHandler* music_h, float vol);
int music_seek(MusicHandler* music_h, float position);
float music_get_pos(MusicHandler* music_h);
int music_is_playing(MusicHandler* music_h);
void music_stop(MusicHandler* music_h);
int music_play(MusicHandler* music_h);

#endif