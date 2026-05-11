#include <Arduino.h>
#include <cstring>
#include "OrderEventHandler.h"
#include "StatusIndicator.h"
#include "NotificationSound.h"

OrderEventHandler::OrderEventHandler(StatusIndicator *indicator, NotificationSound *buzzer)
    : _indicator(indicator), _buzzer(buzzer) {}

void OrderEventHandler::handle(const OrderEvent &evt)
{
    Serial.printf(">>> Order #%d is now: %s\n", evt.order, evt.status);
    _indicator->update(evt.status);

    if (strcmp(evt.status, "ready") == 0)
        _buzzer->playOrderReady();
}
