#include "PayloadParser.h"
#include <ArduinoJson.h>
#include <cstring>

namespace PayloadParser
{
    static void copyOrDefault(char *dst, size_t dstSize, const char *src)
    {
        strncpy(dst, src, dstSize - 1);
        dst[dstSize - 1] = '\0';
    }

    bool parse(const uint8_t *payload, size_t length, OrderEvent &out)
    {
        JsonDocument doc;
        DeserializationError err = deserializeJson(doc, payload, length);
        if (err)
            return false;

        out.order = doc["order"] | 0;
        copyOrDefault(out.status, sizeof(out.status), doc["status"] | "unknown");
        copyOrDefault(out.eta,    sizeof(out.eta),    doc["eta"]    | "-");
        return true;
    }
}
