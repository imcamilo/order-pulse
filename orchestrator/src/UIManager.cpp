#include <Arduino.h>
#include "UIManager.h"
#include "Config.h"
#include "Labels.h"

UIManager::UIManager(TFT_eSPI *tft)
    : _tft(tft),
      _onStatusSelected(nullptr),
      _screen(TouchLogic::Screen::TABLE_LIST),
      _selectedTable(-1),
      _overlayShownAt(0) {}

void UIManager::begin(StatusSelectedFn onStatusSelected)
{
    _onStatusSelected = onStatusSelected;
    drawTableList();
}

void UIManager::drawButton(const TouchLogic::Button &b, const char *label)
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
    _tft->drawString(Labels::PANEL_TITLE, 160, 25, 4);
    for (int i = 0; i < Config::NUM_TABLES; i++)
        drawButton(TouchLogic::TABLE_BTNS[i], Config::TABLE_NAMES[i]);
}

void UIManager::drawTableDetail(int table)
{
    _tft->fillScreen(TFT_BLACK);
    drawButton(TouchLogic::BACK_BTN, Labels::BACK_BUTTON);

    char title[16];
    snprintf(title, sizeof(title), Labels::FMT_TABLE_DETAIL, table);
    _tft->setTextColor(TFT_WHITE, TFT_BLACK);
    _tft->setTextDatum(MC_DATUM);
    _tft->drawString(title, 200, 22, 4);

    drawButton(TouchLogic::STATUS_BTNS[0], TouchLogic::STATUS_DISPLAY[0]);
    drawButton(TouchLogic::STATUS_BTNS[1], TouchLogic::STATUS_DISPLAY[1]);
    drawButton(TouchLogic::STATUS_BTNS[2], TouchLogic::STATUS_DISPLAY[2]);
}

void UIManager::drawSentOverlay(const char *status)
{
    _tft->fillRect(40, 80, 240, 80, TFT_DARKGREEN);
    _tft->drawRect(40, 80, 240, 80, TFT_WHITE);
    _tft->setTextColor(TFT_WHITE, TFT_DARKGREEN);
    _tft->setTextDatum(MC_DATUM);
    _tft->drawString(Labels::SENT_OVERLAY, 160, 110, 4);
    _tft->drawString(status, 160, 140, 2);
}

void UIManager::handleTouch(int x, int y)
{
    using namespace TouchLogic;
    Action action = resolveTouch(_screen, x, y);

    switch (action.type)
    {
    case ActionType::SELECT_TABLE:
        _selectedTable = action.tableId;
        _screen = Screen::TABLE_DETAIL;
        drawTableDetail(_selectedTable);
        break;

    case ActionType::GO_BACK:
        _selectedTable = -1;
        _screen = Screen::TABLE_LIST;
        drawTableList();
        break;

    case ActionType::SELECT_STATUS:
        if (_onStatusSelected)
            _onStatusSelected(_selectedTable, STATUS_LABELS[action.statusIdx]);
        drawSentOverlay(STATUS_DISPLAY[action.statusIdx]);
        _screen = Screen::SENT_OVERLAY;
        _overlayShownAt = millis();
        break;

    case ActionType::NONE:
        break;
    }
}

void UIManager::tick()
{
    if (_screen == TouchLogic::Screen::SENT_OVERLAY &&
        millis() - _overlayShownAt > 1000)
    {
        _screen = TouchLogic::Screen::TABLE_LIST;
        _selectedTable = -1;
        drawTableList();
    }
}
