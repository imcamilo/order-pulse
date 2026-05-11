#ifndef NOTIFICATION_SOUND_H
#define NOTIFICATION_SOUND_H

#include <Arduino.h>
#include "StatusLogic.h"

// Non-blocking buzzer: holds a note sequence and advances note by note
// on each tick(). The "which sequence to play" decision lives in StatusLogic,
// keeping this class focused on the LEDC hardware path.
class NotificationSound
{
private:
    using Note = StatusLogic::Note;

    int _pin;
    uint8_t _volume;
    uint8_t _channel;

    static constexpr int MAX_NOTES = 16;
    Note _seq[MAX_NOTES];
    int _seqLen = 0;
    int _seqIdx = 0;
    uint32_t _stepStart = 0;
    bool _stepActive = false;

    void _setSequence(const Note *src, int n)
    {
        if (n > MAX_NOTES)
            n = MAX_NOTES;
        for (int i = 0; i < n; i++)
            _seq[i] = src[i];
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

    // Call from every loop() iteration: advances the melody without blocking.
    void tick()
    {
        if (_seqIdx >= _seqLen)
            return;

        const Note &n = _seq[_seqIdx];

        if (!_stepActive)
        {
            if (n.freq == 0)
            {
                ledcWrite(_channel, 0);
            }
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

    // Plays the "order ready" motif. Ignores re-triggers while playing.
    void playOrderReady()
    {
        if (isPlaying())
            return;
        int len;
        const Note *seq = StatusLogic::readyMotif(len);
        _setSequence(seq, len);
    }
};

#endif
