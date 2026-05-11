#include "TouchLogic.h"

namespace TouchLogic
{
    // RGB565 colors matching TFT_eSPI macros (TFT_DARKGREY/BLUE/ORANGE/GREEN).
    static constexpr uint16_t COL_DARKGREY = 0x7BEF;
    static constexpr uint16_t COL_BLUE     = 0x001F;
    static constexpr uint16_t COL_ORANGE   = 0xFD20;
    static constexpr uint16_t COL_GREEN    = 0x07E0;

    const Button TABLE_BTNS[4] = {
        {30,  60, 110, 60, COL_DARKGREY},
        {180, 60, 110, 60, COL_DARKGREY},
        {30, 150, 110, 60, COL_DARKGREY},
        {180, 150, 110, 60, COL_DARKGREY}
    };

    const Button STATUS_BTNS[3] = {
        {60,  60, 200, 40, COL_BLUE},
        {60, 110, 200, 40, COL_ORANGE},
        {60, 160, 200, 40, COL_GREEN}
    };

    const Button BACK_BTN = {5, 5, 70, 35, COL_DARKGREY};

    const char *const STATUS_LABELS[3]  = {"pending", "preparing", "ready"};
    const char *const STATUS_DISPLAY[3] = {"PENDIENTE", "PREPARANDO", "LISTO"};

    bool hitTest(const Button &b, int x, int y)
    {
        return x >= b.x && x < b.x + b.w && y >= b.y && y < b.y + b.h;
    }

    Action resolveTouch(Screen current, int x, int y)
    {
        Action a{ActionType::NONE, 0, 0};

        if (current == Screen::TABLE_LIST)
        {
            for (int i = 0; i < 4; i++)
            {
                if (hitTest(TABLE_BTNS[i], x, y))
                {
                    a.type = ActionType::SELECT_TABLE;
                    a.tableId = i + 1;
                    return a;
                }
            }
            return a;
        }

        if (current == Screen::TABLE_DETAIL)
        {
            if (hitTest(BACK_BTN, x, y))
            {
                a.type = ActionType::GO_BACK;
                return a;
            }
            for (int i = 0; i < 3; i++)
            {
                if (hitTest(STATUS_BTNS[i], x, y))
                {
                    a.type = ActionType::SELECT_STATUS;
                    a.statusIdx = i;
                    return a;
                }
            }
            return a;
        }

        // SENT_OVERLAY: ignore touches
        return a;
    }
}
