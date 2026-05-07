#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <TFT_eSPI.h>

class UIManager
{
public:
    // Callback que dispara la UI cuando el usuario selecciona un estado.
    // tableId es 1..NUM_TABLES, status es uno de "pending"|"preparing"|"ready".
    typedef void (*StatusSelectedFn)(int tableId, const char *status);

private:
    TFT_eSPI *_tft;
    StatusSelectedFn _onStatusSelected;

    enum class Screen
    {
        TABLE_LIST,
        TABLE_DETAIL,
        SENT_OVERLAY
    };

    Screen _screen;
    int _selectedTable;
    uint32_t _overlayShownAt;

    struct Button
    {
        int x, y, w, h;
        uint16_t color;
    };

    static const Button TABLE_BTNS[4];
    static const Button STATUS_BTNS[3];
    static const Button BACK_BTN;
    // Lo que se publica por MQTT (no traducir: el subscriber espera estos strings)
    static const char *STATUS_LABELS[3];
    // Lo que se muestra en pantalla
    static const char *STATUS_DISPLAY[3];

    bool hitTest(const Button &b, int x, int y) const;
    void drawButton(const Button &b, const char *label);
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
