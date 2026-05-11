#ifndef STATUS_INDICATOR_H
#define STATUS_INDICATOR_H

// CYD discrete RGB LED (common-anode — LOW turns on, HIGH turns off).
// Maps states to colors: pending=blue, preparing=yellow, ready=green.
class StatusIndicator
{
private:
    int _pinR, _pinG, _pinB;
    void _set(bool r, bool g, bool b);

public:
    StatusIndicator(int pinR, int pinG, int pinB);
    void begin();
    void update(const char *status);
};

#endif
