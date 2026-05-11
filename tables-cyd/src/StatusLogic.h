#ifndef STATUS_LOGIC_H
#define STATUS_LOGIC_H

#include <cstdint>
#include <cstring>

// Pure decision functions for the CYD-based table firmware (no hardware
// dependencies). Maps status strings to RGB triplets, display visuals, and
// note sequences. The hardware classes (StatusIndicator, StatusDisplay,
// NotificationSound) consume these helpers.
namespace StatusLogic
{
    // Discrete RGB triplet (each channel either on or off).
    struct RgbTriplet { bool r, g, b; };

    // Display visual: label string and TFT color (RGB565 packed).
    struct Visual { const char *label; uint16_t color; };

    // Tone (freq=0 means silence).
    struct Note { uint32_t freq; uint32_t durMs; };

    RgbTriplet rgbForStatus(const char *status);
    Visual visualForStatus(const char *status);

    // Returns a pointer to a static note sequence and writes its length to
    // outLen. Returns nullptr (outLen = 0) for unknown statuses.
    const Note *sequenceForStatus(const char *status, int &outLen);
}

#endif
