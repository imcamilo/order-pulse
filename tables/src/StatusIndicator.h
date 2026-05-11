#ifndef STATUS_INDICATOR_H
#define STATUS_INDICATOR_H

#include <Adafruit_NeoPixel.h>
#include "IStatusIndicator.h"

class StatusIndicator : public IStatusIndicator
{
private:
    Adafruit_NeoPixel _led;

public:
    StatusIndicator(int pin, int numPixels);
    void begin();
    void update(const char *status) override;
};

#endif
