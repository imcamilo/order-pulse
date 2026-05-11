#include <Arduino.h>
#include "StatusIndicator.h"
#include "StatusLogic.h"

StatusIndicator::StatusIndicator(int pinR, int pinG, int pinB)
    : _pinR(pinR), _pinG(pinG), _pinB(pinB) {}

void StatusIndicator::begin()
{
    pinMode(_pinR, OUTPUT);
    pinMode(_pinG, OUTPUT);
    pinMode(_pinB, OUTPUT);
    _set(false, false, false); // off at start
}

void StatusIndicator::_set(bool r, bool g, bool b)
{
    digitalWrite(_pinR, r ? LOW : HIGH);
    digitalWrite(_pinG, g ? LOW : HIGH);
    digitalWrite(_pinB, b ? LOW : HIGH);
}

void StatusIndicator::update(const char *status)
{
    StatusLogic::RgbTriplet t = StatusLogic::rgbForStatus(status);
    _set(t.r, t.g, t.b);
}
