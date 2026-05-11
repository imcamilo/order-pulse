#ifndef NOTIFICATION_SOUND_H
#define NOTIFICATION_SOUND_H

#include <Arduino.h>

// Non-blocking buzzer: builds a note sequence and plays it advancing
// note by note on each tick(). playForStatus() ignores retriggers while playing.
class NotificationSound
{
private:
    int _pin;
    uint8_t _volume;
    uint8_t _channel;

    struct Note { uint32_t freq; uint32_t durMs; };  // freq=0 means silence
    static constexpr int MAX_NOTES = 16;

    Note _seq[MAX_NOTES];
    int _seqLen = 0;
    int _seqIdx = 0;
    uint32_t _stepStart = 0;
    bool _stepActive = false;

    void _setSequence(const Note *src, int n)
    {
        if (n > MAX_NOTES) n = MAX_NOTES;
        for (int i = 0; i < n; i++) _seq[i] = src[i];
        _seqLen = n;
        _seqIdx = 0;
        _stepActive = false;
    }

public:
    NotificationSound(int pin, uint8_t volume, uint8_t channel = 0)
        : _pin(pin), _volume(volume), _channel(channel) {}

    void begin()
    {
        ledcSetup(_channel, 1000, 8);
        ledcAttachPin(_pin, _channel);
        ledcWrite(_channel, 0);
    }

    bool isPlaying() const { return _seqIdx < _seqLen; }

    void tick()
    {
        if (_seqIdx >= _seqLen) return;
        const Note &n = _seq[_seqIdx];

        if (!_stepActive)
        {
            if (n.freq == 0)
                ledcWrite(_channel, 0);
            else
            {
                ledcWriteTone(_channel, n.freq);
                ledcWrite(_channel, _volume);
            }
            _stepStart = millis();
            _stepActive = true;
        }

        if (millis() - _stepStart >= n.durMs)
        {
            ledcWrite(_channel, 0);
            _stepActive = false;
            _seqIdx++;
        }
    }

    // Dispatches the sound matching the given status.
    void playForStatus(const char *status)
    {
        if (isPlaying()) return;

        if (strcmp(status, "pending") == 0)
        {
            static const Note seq[] = {{1500, 200}};
            _setSequence(seq, 1);
        }
        else if (strcmp(status, "preparing") == 0)
        {
            static const Note seq[] = {
                {2000, 150}, {0, 80}, {2000, 150}
            };
            _setSequence(seq, 3);
        }
        else if (strcmp(status, "ready") == 0)
        {
            // Beethoven 5th, fate motif: G G G Eb (G6/Eb6)
            static const Note seq[] = {
                {1568, 150}, {0, 50},
                {1568, 150}, {0, 50},
                {1568, 150}, {0, 50},
                {1244, 800}
            };
            _setSequence(seq, 7);
        }
    }
};

#endif
