#include "StatusDisplay.h"

// Coordinates in 320x240 landscape
static constexpr int HEADER_Y     = 30;
static constexpr int BOX_X        = 20;
static constexpr int BOX_Y        = 80;
static constexpr int BOX_W        = 280;
static constexpr int BOX_H        = 80;
static constexpr int FOOTER_Y     = 200;

StatusDisplay::StatusDisplay(TFT_eSPI *tft, int tableId)
    : _tft(tft), _tableId(tableId) {}

void StatusDisplay::begin()
{
    _tft->fillScreen(TFT_BLACK);
    drawHeader();
    showWaiting();
}

void StatusDisplay::drawHeader()
{
    char buf[16];
    snprintf(buf, sizeof(buf), "MESA %d", _tableId);
    _tft->setTextDatum(MC_DATUM);
    _tft->setTextColor(TFT_WHITE, TFT_BLACK);
    _tft->drawString(buf, 160, HEADER_Y, 6); // font 6, large
}

void StatusDisplay::drawStatusBox(const char *statusUpper, uint16_t bgColor)
{
    _tft->fillRoundRect(BOX_X, BOX_Y, BOX_W, BOX_H, 8, bgColor);
    _tft->drawRoundRect(BOX_X, BOX_Y, BOX_W, BOX_H, 8, TFT_WHITE);
    _tft->setTextDatum(MC_DATUM);
    _tft->setTextColor(TFT_WHITE, bgColor);
    _tft->drawString(statusUpper, BOX_X + BOX_W / 2, BOX_Y + BOX_H / 2, 4);
}

void StatusDisplay::showWaiting()
{
    drawStatusBox("ESPERANDO PEDIDO", TFT_DARKGREY);
    // clear footer
    _tft->fillRect(0, FOOTER_Y - 10, 320, 50, TFT_BLACK);
}

void StatusDisplay::update(const char *status, int orderId, const char *eta)
{
    const char *display;
    uint16_t color;

    if (strcmp(status, "pending") == 0)
    {
        display = "PENDIENTE";
        color = TFT_BLUE;
    }
    else if (strcmp(status, "preparing") == 0)
    {
        display = "PREPARANDO";
        color = TFT_ORANGE;
    }
    else if (strcmp(status, "ready") == 0)
    {
        display = "LISTO";
        color = TFT_DARKGREEN;
    }
    else
    {
        display = "DESCONOCIDO";
        color = TFT_RED;
    }

    drawStatusBox(display, color);

    // Footer in two lines (font 2 small, ~16px tall)
    _tft->fillRect(0, FOOTER_Y - 5, 320, 50, TFT_BLACK);
    _tft->setTextDatum(MC_DATUM);
    _tft->setTextColor(TFT_WHITE, TFT_BLACK);

    char line1[32];
    char line2[48];
    snprintf(line1, sizeof(line1), "Orden #%d", orderId);
    snprintf(line2, sizeof(line2), "Tiempo estimado: %s", eta);

    _tft->drawString(line1, 160, FOOTER_Y, 2);
    _tft->drawString(line2, 160, FOOTER_Y + 18, 2);
}
