#include "Touch.h"
#include "Config.h"

Touch::Touch()
    : _spi(VSPI),
      _ts(Config::PIN_TOUCH_CS, Config::PIN_TOUCH_IRQ),
      _lastReadAt(0) {}

void Touch::begin()
{
    _spi.begin(Config::PIN_TOUCH_CLK,
               Config::PIN_TOUCH_MISO,
               Config::PIN_TOUCH_MOSI,
               Config::PIN_TOUCH_CS);
    _ts.begin(_spi);
    _ts.setRotation(1); // landscape
}

bool Touch::readPoint(int &x, int &y)
{
    const uint32_t now = millis();
    if (now - _lastReadAt < 200)
        return false;

    if (!_ts.touched())
        return false;

    _lastReadAt = now;

    TS_Point p = _ts.getPoint();
    x = map(p.x, RAW_X_MIN, RAW_X_MAX, 0, 320);
    y = map(p.y, RAW_Y_MIN, RAW_Y_MAX, 0, 240);
    x = constrain(x, 0, 319);
    y = constrain(y, 0, 239);

    Serial.printf("[Touch] raw=(%d,%d) screen=(%d,%d)\n", p.x, p.y, x, y);
    return true;
}
