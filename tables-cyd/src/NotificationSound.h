#ifndef NOTIFICATION_SOUND_H
#define NOTIFICATION_SOUND_H

#include <Arduino.h>
#include "INotificationSound.h"
#include "StatusLogic.h"

// Non-blocking buzzer. The "which sequence to play" decision lives in
// StatusLogic; this class only knows about timing and LEDC hardware.
class NotificationSound : public INotificationSound
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

    // Plays the sequence matching the given status. Ignores re-triggers
    // while a sequence is in progress, and unknown statuses (nullptr).
    void playForStatus(const char *status) override
    {
        if (isPlaying()) return;
        int len;
        const Note *seq = StatusLogic::sequenceForStatus(status, len);
        if (seq != nullptr)
            _setSequence(seq, len);
    }
};

#endif
