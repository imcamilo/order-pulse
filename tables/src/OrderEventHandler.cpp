#include <Arduino.h>
#include <cstring>
#include "OrderEventHandler.h"
#include "IStatusIndicator.h"
#include "INotificationSound.h"

OrderEventHandler::OrderEventHandler(IStatusIndicator *indicator, INotificationSound *buzzer)
    : _indicator(indicator), _buzzer(buzzer) {}

void OrderEventHandler::handle(const OrderEvent &evt)
{
    Serial.printf("[Handler] Order #%d -> %s\n", evt.order, evt.status);
    _indicator->update(evt.status);

    if (strcmp(evt.status, "ready") == 0)
        _buzzer->playOrderReady();
}
