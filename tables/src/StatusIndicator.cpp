#include "StatusIndicator.h"
#include "StatusLogic.h"

StatusIndicator::StatusIndicator(int pin, int numPixels)
    : _led(numPixels, pin, NEO_GRB + NEO_KHZ800) {}

void StatusIndicator::begin()
{
    _led.begin();
    _led.setBrightness(40);
    _led.setPixelColor(0, _led.Color(50, 50, 50));
    _led.show();
}

void StatusIndicator::update(const char *status)
{
    StatusLogic::RgbColor c = StatusLogic::rgbForStatus(status);
    _led.setPixelColor(0, _led.Color(c.r, c.g, c.b));
    _led.show();
}
