#include "StatusIndicator.h"

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
    if (strcmp(status, "pending") == 0)
        _led.setPixelColor(0, _led.Color(0, 0, 255));
    else if (strcmp(status, "preparing") == 0)
        _led.setPixelColor(0, _led.Color(255, 100, 0));
    else if (strcmp(status, "ready") == 0)
        _led.setPixelColor(0, _led.Color(0, 255, 0));
    else
        _led.setPixelColor(0, _led.Color(255, 0, 0));
    _led.show();
}