#ifndef PAYLOAD_PARSER_H
#define PAYLOAD_PARSER_H

#include <cstdint>
#include <cstddef>

// Parsed payload of an order status MQTT message.
struct OrderEvent
{
    int order;
    char status[16];
    char eta[16];
};

// Pure JSON parsing — no hardware dependency.
namespace PayloadParser
{
    // Parses a byte buffer into an OrderEvent. Returns false on JSON error.
    // Missing fields fall back to defaults (status="unknown", eta="-", order=0).
    bool parse(const uint8_t *payload, size_t length, OrderEvent &out);
}

#endif
