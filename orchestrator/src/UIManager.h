#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <TFT_eSPI.h>
#include "TouchLogic.h"

// Renders the control panel UI on the TFT and dispatches touch events.
// State transitions and button geometry live in TouchLogic; this class
// owns drawing, the StatusSelectedFn callback, and the SENT_OVERLAY
// timer.
class UIManager
{
public:
    typedef void (*StatusSelectedFn)(int tableId, const char *status);

private:
    TFT_eSPI *_tft;
    StatusSelectedFn _onStatusSelected;

    TouchLogic::Screen _screen;
    int _selectedTable;
    uint32_t _overlayShownAt;

    void drawButton(const TouchLogic::Button &b, const char *label);
    void drawTableList();
    void drawTableDetail(int table);
    void drawSentOverlay(const char *status);

public:
    UIManager(TFT_eSPI *tft);
    void begin(StatusSelectedFn onStatusSelected);
    void handleTouch(int x, int y);
    void tick();
};

#endif
