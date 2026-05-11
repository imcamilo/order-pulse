#include <Arduino.h>
#include "StatusIndicator.h"

StatusIndicator::StatusIndicator(int pinR, int pinG, int pinB)
    : _pinR(pinR), _pinG(pinG), _pinB(pinB) {}

void StatusIndicator::begin()
{
    pinMode(_pinR, OUTPUT);
    pinMode(_pinG, OUTPUT);
    pinMode(_pinB, OUTPUT);
    _set(false, false, false); // off at start
}

// r/g/b true = on (writes LOW for common-anode)
void StatusIndicator::_set(bool r, bool g, bool b)
{
    digitalWrite(_pinR, r ? LOW : HIGH);
    digitalWrite(_pinG, g ? LOW : HIGH);
    digitalWrite(_pinB, b ? LOW : HIGH);
}

void StatusIndicator::update(const char *status)
{
    if (strcmp(status, "pending") == 0)
        _set(false, false, true);  // blue
    else if (strcmp(status, "preparing") == 0)
        _set(true, true, false);   // yellow (R+G)
    else if (strcmp(status, "ready") == 0)
        _set(false, true, false);  // green
    else
        _set(true, false, false);  // red (unknown status)
}
