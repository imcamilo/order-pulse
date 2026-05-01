#ifndef STATUS_INDICATOR_H
#define STATUS_INDICATOR_H

#include <Adafruit_NeoPixel.h>

class StatusIndicator
{
private:
    Adafruit_NeoPixel _led;

public:
    StatusIndicator(int pin, int numPixels);
    void begin();
    void update(const char *status);
};

#endif