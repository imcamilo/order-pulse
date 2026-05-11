#ifndef TOUCH_LOGIC_H
#define TOUCH_LOGIC_H

#include <cstdint>

// Pure decision layer for the control panel (no hardware dependencies).
// Owns the screen enum, button geometry, and the touch->action mapping.
// The UIManager hardware class consumes these helpers and only handles
// drawing and side effects.
namespace TouchLogic
{
    enum class Screen
    {
        TABLE_LIST,
        TABLE_DETAIL,
        SENT_OVERLAY
    };

    enum class ActionType
    {
        NONE,
        SELECT_TABLE,   // user picked a table on the list
        GO_BACK,        // back button on the detail screen
        SELECT_STATUS   // user picked a status on the detail screen
    };

    struct Action
    {
        ActionType type;
        int tableId;    // valid when type == SELECT_TABLE
        int statusIdx;  // valid when type == SELECT_STATUS (0..2)
    };

    // Button geometry. color is RGB565 (matches TFT_eSPI encoding).
    struct Button { int x, y, w, h; uint16_t color; };

    extern const Button TABLE_BTNS[4];
    extern const Button STATUS_BTNS[3];
    extern const Button BACK_BTN;

    // MQTT payload values (English: subscribers depend on these strings)
    extern const char *const STATUS_LABELS[3];
    // Spanish UI labels shown on the screen
    extern const char *const STATUS_DISPLAY[3];

    bool hitTest(const Button &b, int x, int y);
    Action resolveTouch(Screen current, int x, int y);
}

#endif
