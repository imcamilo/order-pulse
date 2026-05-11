#ifndef STATUS_INDICATOR_H
#define STATUS_INDICATOR_H

#include "IStatusIndicator.h"

// CYD discrete RGB LED (common-anode — LOW turns on, HIGH turns off).
class StatusIndicator : public IStatusIndicator
{
private:
    int _pinR, _pinG, _pinB;
    void _set(bool r, bool g, bool b);

public:
    StatusIndicator(int pinR, int pinG, int pinB);
    void begin();
    void update(const char *status) override;
};

#endif
