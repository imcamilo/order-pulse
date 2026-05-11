#include "StatusLogic.h"
#include "Labels.h"

namespace StatusLogic
{
    // RGB565 color constants used by the TFT layer.
    // Match the values of TFT_eSPI's TFT_BLUE/TFT_ORANGE/TFT_DARKGREEN/TFT_RED.
    static constexpr uint16_t COL_BLUE       = 0x001F;
    static constexpr uint16_t COL_GREEN_DARK = 0x03E0;
    static constexpr uint16_t COL_ORANGE     = 0xFD20;
    static constexpr uint16_t COL_RED        = 0xF800;

    RgbTriplet rgbForStatus(const char *status)
    {
        if (strcmp(status, "pending") == 0)
            return {false, false, true};   // blue
        if (strcmp(status, "preparing") == 0)
            return {true, true, false};    // yellow (R+G)
        if (strcmp(status, "ready") == 0)
            return {false, true, false};   // green
        return {true, false, false};       // red (unknown)
    }

    Visual visualForStatus(const char *status)
    {
        if (strcmp(status, "pending") == 0)
            return {Labels::STATUS_PENDING, COL_BLUE};
        if (strcmp(status, "preparing") == 0)
            return {Labels::STATUS_PREPARING, COL_ORANGE};
        if (strcmp(status, "ready") == 0)
            return {Labels::STATUS_READY, COL_GREEN_DARK};
        return {Labels::STATUS_UNKNOWN, COL_RED};
    }

    static const Note PENDING_SEQ[] = {{1500, 200}};
    static const Note PREPARING_SEQ[] = {{2000, 150}, {0, 80}, {2000, 150}};
    // Beethoven 5th, fate motif: G G G Eb (G6/Eb6).
    static const Note READY_SEQ[] = {
        {1568, 150}, {0, 50},
        {1568, 150}, {0, 50},
        {1568, 150}, {0, 50},
        {1244, 800}
    };

    const Note *sequenceForStatus(const char *status, int &outLen)
    {
        if (strcmp(status, "pending") == 0)
        {
            outLen = sizeof(PENDING_SEQ) / sizeof(PENDING_SEQ[0]);
            return PENDING_SEQ;
        }
        if (strcmp(status, "preparing") == 0)
        {
            outLen = sizeof(PREPARING_SEQ) / sizeof(PREPARING_SEQ[0]);
            return PREPARING_SEQ;
        }
        if (strcmp(status, "ready") == 0)
        {
            outLen = sizeof(READY_SEQ) / sizeof(READY_SEQ[0]);
            return READY_SEQ;
        }
        outLen = 0;
        return nullptr;
    }
}
