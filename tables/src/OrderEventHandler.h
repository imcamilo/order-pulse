#ifndef ORDER_EVENT_HANDLER_H
#define ORDER_EVENT_HANDLER_H

#include "PayloadParser.h"

class StatusIndicator;
class NotificationSound;

// Reacts to a parsed order event by driving the table's peripherals.
// Decoupled from MQTT/JSON so the parsing can be tested in isolation.
class OrderEventHandler
{
private:
    StatusIndicator *_indicator;
    NotificationSound *_buzzer;

public:
    OrderEventHandler(StatusIndicator *indicator, NotificationSound *buzzer);
    void handle(const OrderEvent &evt);
};

#endif
