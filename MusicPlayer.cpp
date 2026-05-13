#include "MusicPlayer.h"
#include "Songs.h"

MusicPlayer::MusicPlayer(int pin)
: _pin(pin) {}

void MusicPlayer::begin(int songId) {
    pinMode(_pin, OUTPUT);
    loadSong(songId);
    _index = 0;
    _playing = false;
    _finished = false;   // reset finished flag
}

void MusicPlayer::loadSong(int id) {
    _songId = id;

    switch (id) {
        case SONG_MARIO:
            _melody = marioMelody;
            _durations = marioDurations;
            _length = marioLength;
            _wholenote = marioWholenote;
            break;

        case SONG_MII:
            _melody = miiMelody;
            _durations = miiDurations;
            _length = miiLength;
            _wholenote = miiWholenote;
            break;

        case SONG_GREENHILL:
            _melody = greenHillMelody;
            _durations = greenHillDurations;
            _length = greenHillLength;
            _wholenote = greenHillWholenote;
            break;

        default:
            _melody = nullptr;
            _durations = nullptr;
            _length = 0;
            break;
    }
}

void MusicPlayer::update() {
    if (_melody == nullptr || _finished) return;

    unsigned long now = millis();

    if (!_playing) {
        int divider = _durations[_index];
        int duration = (divider > 0)
            ? (_wholenote / divider)
            : (_wholenote / abs(divider) * 1.5);

        int playTime = duration * 0.9;

        int freq = _melody[_index];
        if (freq != 0) {
            tone(_pin, freq, playTime);
        }

        _noteStart = now;
        _playing = true;
        return;
    }

    int divider = _durations[_index];
    int duration = (divider > 0)
        ? (_wholenote / divider)
        : (_wholenote / abs(divider) * 1.5);

    if (now - _noteStart >= duration) {
        noTone(_pin);
        _playing = false;

        _index++;

        // STOP after last note
        if (_index >= _length) {
            _finished = true;
            return;
        }
    }
}

void MusicPlayer::reset() {
    _index = 0;
    _playing = false;
    _finished = false;
    noTone(_pin);
}

