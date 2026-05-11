#include <Arduino.h>
#include "OrderEventHandler.h"
#include "StatusIndicator.h"
#include "StatusDisplay.h"
#include "NotificationSound.h"

OrderEventHandler::OrderEventHandler(StatusIndicator *indicator,
                                     StatusDisplay *display,
                                     NotificationSound *buzzer)
    : _indicator(indicator), _display(display), _buzzer(buzzer) {}

void OrderEventHandler::handle(const OrderEvent &evt)
{
    Serial.printf(">>> Order #%d -> %s (eta %s)\n", evt.order, evt.status, evt.eta);
    _display->update(evt.status, evt.order, evt.eta);
    _indicator->update(evt.status);
    _buzzer->playForStatus(evt.status);
}
