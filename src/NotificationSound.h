#ifndef NOTIFICATION_SOUND_H
#define NOTIFICATION_SOUND_H

#include <Arduino.h>

class NotificationSound
{
private:
    int _pin;
    uint8_t _volume;
    uint8_t _channel;

    void _play(uint32_t freq, uint32_t durationMs)
    {
        ledcWriteTone(_channel, freq);
        ledcWrite(_channel, _volume);
        delay(durationMs);
        ledcWrite(_channel, 0);
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

    void playOrderReady()
    {
        for (int i = 0; i < 4; i++)
        {
            _play(1500, 150);
            delay(50);
            _play(2500, 300);
            if (i < 3)
                delay(200);
        }
    }

    void playError()
    {
        _play(400, 500);
    }
};

#endif
