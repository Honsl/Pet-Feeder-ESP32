#pragma once
#include <Arduino.h>

class MusicPlayer {
public:
    MusicPlayer(int pin);

    void begin(int songId);
    void update();
    void reset();
    bool isFinished() const { return _finished; }

private:
    int _pin;
    int _songId;

    const int* _melody;
    const int* _durations;
    int _length;
    int _wholenote;

    unsigned long _noteStart = 0;
    int _index = 0;
    bool _playing = false;
    bool _finished = false;   // NEW

    void loadSong(int id);
};
