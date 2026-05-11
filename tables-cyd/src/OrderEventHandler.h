#ifndef ORDER_EVENT_HANDLER_H
#define ORDER_EVENT_HANDLER_H

#include "PayloadParser.h"

class IStatusIndicator;
class IStatusDisplay;
class INotificationSound;

// Reacts to a parsed order event by driving the table's peripherals.
// Accepts abstract interfaces so tests can inject mocks.
class OrderEventHandler
{
private:
    IStatusIndicator *_indicator;
    IStatusDisplay *_display;
    INotificationSound *_buzzer;

public:
    OrderEventHandler(IStatusIndicator *indicator,
                      IStatusDisplay *display,
                      INotificationSound *buzzer);
    void handle(const OrderEvent &evt);
};

#endif
