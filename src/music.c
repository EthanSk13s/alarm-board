#include <raylib.h>
#include <stdlib.h>

#include "music.h"

int music_init(MusicHandler* music_h, char* filepath)
{
    if (filepath == NULL || music_h == NULL)
    {
        return -1;
    }

    music_h->music = LoadMusicStream(filepath);
    music_h->volume = 0.5;
    SetMusicVolume(music_h->music, 0.5);

    return 0;
}

void music_update(MusicHandler* music_h)
{
    if (IsMusicStreamPlaying(music_h->music))
    {
        UpdateMusicStream(music_h->music);
    }
}

void music_set_vol(MusicHandler* music_h, float vol)
{
    if (vol >= 1.0)
    {
        vol = 1.0;
    } else if (vol <= 0)
    {
        vol = 0.f;
    }

    music_h->volume = vol;
    SetMusicVolume(music_h->music, vol);
}

int music_seek(MusicHandler* music_h, float position)
{
    if (!music_is_playing(music_h))
    {
        return -1;
    }

    SeekMusicStream(music_h->music, position);

    return position;
}

float music_get_pos(MusicHandler* music_h)
{
    return GetMusicTimePlayed(music_h->music);
}

int music_is_playing(MusicHandler* music_h)
{
    return IsMusicStreamPlaying(music_h->music);
}

void music_stop(MusicHandler* music_h)
{
    StopMusicStream(music_h->music);
}

void music_play(MusicHandler* music_h)
{
    PlayMusicStream(music_h->music);
}