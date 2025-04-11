#include "alarm.h"

struct AlarmMusicManager
{
    Music music;
};

struct AlarmMusicManager alarm_man;

int load_alarm()
{
    alarm_man.music = LoadMusicStream("assets/alarm.mp3");
    SetMusicVolume(alarm_man.music, 0.25);

    return IsMusicValid(alarm_man.music);
}

int is_alarm_playing()
{
    return IsMusicStreamPlaying(alarm_man.music);
}

void play_alarm()
{
    PlayMusicStream(alarm_man.music);
}

void stop_alarm()
{
    StopMusicStream(alarm_man.music);
}

void update_stream()
{
    if (IsMusicStreamPlaying(alarm_man.music))
    {
        UpdateMusicStream(alarm_man.music);
    }
}