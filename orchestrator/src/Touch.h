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

    // Calibración aproximada para CYD v1 en rotación landscape (rot=1).
    // Si el touch responde fuera del botón, ajusta estos valores.
    static constexpr int RAW_X_MIN = 200;
    static constexpr int RAW_X_MAX = 3700;
    static constexpr int RAW_Y_MIN = 240;
    static constexpr int RAW_Y_MAX = 3800;

public:
    Touch();
    void begin();

    // Devuelve true si hay un toque y rellena (x, y) en coords de pantalla
    // (320x240 landscape). Hace debounce interno de 200ms.
    bool readPoint(int &x, int &y);
};

#endif
