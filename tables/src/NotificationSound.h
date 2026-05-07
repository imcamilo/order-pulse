#ifndef NOTIFICATION_SOUND_H
#define NOTIFICATION_SOUND_H

#include <Arduino.h>

class NotificationSound
{
private:
    int _pin;
    uint8_t _volume;
    uint8_t _channel;

    // Una nota de la melodía: freq=0 significa silencio.
    struct Note
    {
        uint32_t freq;
        uint32_t durMs;
    };

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

    // Llamar en cada iteración de loop(): avanza la melodía sin bloquear.
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

    // Beethoven 5ta, motivo del destino: G G G Eb (subido una octava).
    // Si ya está sonando, ignora el re-trigger.
    void playOrderReady()
    {
        if (isPlaying())
            return;
        static const Note motif[] = {
            {1568, 150}, {0, 50},
            {1568, 150}, {0, 50},
            {1568, 150}, {0, 50},
            {1244, 800}
        };
        _setSequence(motif, sizeof(motif) / sizeof(motif[0]));
    }

    void playError()
    {
        if (isPlaying())
            return;
        static const Note seq[] = {{400, 500}};
        _setSequence(seq, sizeof(seq) / sizeof(seq[0]));
    }
};

#endif
