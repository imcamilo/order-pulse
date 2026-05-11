#ifndef STATUS_DISPLAY_H
#define STATUS_DISPLAY_H

#include <TFT_eSPI.h>
#include "IStatusDisplay.h"

// Draws the order status on the CYD screen.
// Layout (320x240 landscape):
//   - Top header: "MESA N" in a large font
//   - Center box: status name (PENDIENTE/PREPARANDO/LISTO) on a colored background
//   - Bottom line: "Orden #X" + "Tiempo estimado: Y"
class StatusDisplay : public IStatusDisplay
{
private:
    TFT_eSPI *_tft;
    int _tableId;

    void drawHeader();
    void drawStatusBox(const char *statusUpper, uint16_t bgColor);

public:
    StatusDisplay(TFT_eSPI *tft, int tableId);
    void begin();
    void showWaiting();
    void update(const char *status, int orderId, const char *eta) override;
};

#endif
