#ifndef STATUS_LOGIC_H
#define STATUS_LOGIC_H

#include <cstdint>
#include <cstring>

// Pure decision functions for the table firmware (no hardware dependencies).
// Maps status strings ("pending"|"preparing"|"ready"|other) to colors and
// note sequences, so the hardware classes (StatusIndicator, NotificationSound)
// are reduced to thin wrappers around these helpers.
namespace StatusLogic
{
    // 24-bit RGB color for the WS2812 NeoPixel.
    struct RgbColor { uint8_t r, g, b; };

    // One note of a melody (freq=0 means silence).
    struct Note { uint32_t freq; uint32_t durMs; };

    RgbColor rgbForStatus(const char *status);

    // Returns a pointer to the static "order ready" motif and writes its
    // length to outLen.
    const Note *readyMotif(int &outLen);
}

#endif
