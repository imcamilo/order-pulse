#ifndef STATUS_DISPLAY_H
#define STATUS_DISPLAY_H

#include <TFT_eSPI.h>

// Dibuja el estado del pedido en la pantalla CYD.
// Layout (320x240 landscape):
//   - Header arriba: "MESA N" en grande
//   - Caja central: nombre del estado (PENDIENTE/PREPARANDO/LISTO) con color de fondo
//   - Línea inferior: "Orden #X" + "Tiempo estimado: Y"
class StatusDisplay
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
    void update(const char *status, int orderId, const char *eta);
};

#endif
