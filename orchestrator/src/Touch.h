#ifndef TOUCH_H
#define TOUCH_H

#include <SPI.h>
#include <XPT2046_Touchscreen.h>

class Touch
{
private:
    SPIClass _spi;
    XPT2046_Touchscreen _ts;
    uint32_t _lastReadAt;

    // Approximate calibration for CYD v1 in landscape rotation (rot=1).
    // If touch reacts outside the button area, tweak these values.
    static constexpr int RAW_X_MIN = 200;
    static constexpr int RAW_X_MAX = 3700;
    static constexpr int RAW_Y_MIN = 240;
    static constexpr int RAW_Y_MAX = 3800;

public:
    Touch();
    void begin();

    // Returns true if touched and fills (x, y) in screen coordinates
    // (320x240 landscape). Internal 200ms debounce.
    bool readPoint(int &x, int &y);
};

#endif
