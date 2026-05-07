#include "UIManager.h"
#include "Config.h"

const UIManager::Button UIManager::TABLE_BTNS[4] = {
    {30,  60, 110, 60, TFT_DARKGREY},
    {180, 60, 110, 60, TFT_DARKGREY},
    {30, 150, 110, 60, TFT_DARKGREY},
    {180, 150, 110, 60, TFT_DARKGREY}
};

const UIManager::Button UIManager::STATUS_BTNS[3] = {
    {60,  60, 200, 40, TFT_BLUE},
    {60, 110, 200, 40, TFT_ORANGE},
    {60, 160, 200, 40, TFT_GREEN}
};

const UIManager::Button UIManager::BACK_BTN = {5, 5, 70, 35, TFT_DARKGREY};

const char *UIManager::STATUS_LABELS[3]  = {"pending", "preparing", "ready"};
const char *UIManager::STATUS_DISPLAY[3] = {"PENDIENTE", "PREPARANDO", "LISTO"};

UIManager::UIManager(TFT_eSPI *tft)
    : _tft(tft),
      _onStatusSelected(nullptr),
      _screen(Screen::TABLE_LIST),
      _selectedTable(-1),
      _overlayShownAt(0) {}

void UIManager::begin(StatusSelectedFn onStatusSelected)
{
    _onStatusSelected = onStatusSelected;
    drawTableList();
}

bool UIManager::hitTest(const Button &b, int x, int y) const
{
    return x >= b.x && x < b.x + b.w && y >= b.y && y < b.y + b.h;
}

void UIManager::drawButton(const Button &b, const char *label)
{
    _tft->fillRoundRect(b.x, b.y, b.w, b.h, 6, b.color);
    _tft->drawRoundRect(b.x, b.y, b.w, b.h, 6, TFT_WHITE);
    _tft->setTextColor(TFT_WHITE, b.color);
    _tft->setTextDatum(MC_DATUM);
    _tft->drawString(label, b.x + b.w / 2, b.y + b.h / 2, 4);
}

void UIManager::drawTableList()
{
    _tft->fillScreen(TFT_BLACK);
    _tft->setTextColor(TFT_WHITE, TFT_BLACK);
    _tft->setTextDatum(MC_DATUM);
    _tft->drawString("ORDER PULSE - MESAS", 160, 25, 4);
    for (int i = 0; i < Config::NUM_TABLES; i++)
        drawButton(TABLE_BTNS[i], Config::TABLE_NAMES[i]);
}

void UIManager::drawTableDetail(int table)
{
    _tft->fillScreen(TFT_BLACK);
    drawButton(BACK_BTN, "< VOLVER");

    char title[16];
    snprintf(title, sizeof(title), "MESA %d", table);
    _tft->setTextColor(TFT_WHITE, TFT_BLACK);
    _tft->setTextDatum(MC_DATUM);
    _tft->drawString(title, 200, 22, 4);

    drawButton(STATUS_BTNS[0], STATUS_DISPLAY[0]);
    drawButton(STATUS_BTNS[1], STATUS_DISPLAY[1]);
    drawButton(STATUS_BTNS[2], STATUS_DISPLAY[2]);
}

void UIManager::drawSentOverlay(const char *status)
{
    _tft->fillRect(40, 80, 240, 80, TFT_DARKGREEN);
    _tft->drawRect(40, 80, 240, 80, TFT_WHITE);
    _tft->setTextColor(TFT_WHITE, TFT_DARKGREEN);
    _tft->setTextDatum(MC_DATUM);
    _tft->drawString("ENVIADO", 160, 110, 4);
    _tft->drawString(status, 160, 140, 2);
}

void UIManager::handleTouch(int x, int y)
{
    if (_screen == Screen::TABLE_LIST)
    {
        for (int i = 0; i < Config::NUM_TABLES; i++)
        {
            if (hitTest(TABLE_BTNS[i], x, y))
            {
                _selectedTable = i + 1;
                _screen = Screen::TABLE_DETAIL;
                drawTableDetail(_selectedTable);
                return;
            }
        }
    }
    else if (_screen == Screen::TABLE_DETAIL)
    {
        if (hitTest(BACK_BTN, x, y))
        {
            _screen = Screen::TABLE_LIST;
            _selectedTable = -1;
            drawTableList();
            return;
        }
        for (int i = 0; i < 3; i++)
        {
            if (hitTest(STATUS_BTNS[i], x, y))
            {
                if (_onStatusSelected)
                    _onStatusSelected(_selectedTable, STATUS_LABELS[i]);
                drawSentOverlay(STATUS_DISPLAY[i]);
                _screen = Screen::SENT_OVERLAY;
                _overlayShownAt = millis();
                return;
            }
        }
    }
}

void UIManager::tick()
{
    if (_screen == Screen::SENT_OVERLAY && millis() - _overlayShownAt > 1000)
    {
        _screen = Screen::TABLE_LIST;
        _selectedTable = -1;
        drawTableList();
    }
}
