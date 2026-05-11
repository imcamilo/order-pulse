#include "StatusLogic.h"

namespace StatusLogic
{
    RgbColor rgbForStatus(const char *status)
    {
        if (strcmp(status, "pending") == 0)
            return {0, 0, 255};       // blue
        if (strcmp(status, "preparing") == 0)
            return {255, 100, 0};     // orange
        if (strcmp(status, "ready") == 0)
            return {0, 255, 0};       // green
        return {255, 0, 0};           // red (unknown status)
    }

    // Beethoven 5th, fate motif: G G G Eb (one octave up: G6/Eb6).
    static const Note READY_MOTIF[] = {
        {1568, 150}, {0, 50},
        {1568, 150}, {0, 50},
        {1568, 150}, {0, 50},
        {1244, 800}
    };

    const Note *readyMotif(int &outLen)
    {
        outLen = sizeof(READY_MOTIF) / sizeof(READY_MOTIF[0]);
        return READY_MOTIF;
    }
}
