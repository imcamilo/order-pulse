#include <Arduino.h>
#include "OrderEventHandler.h"
#include "IStatusIndicator.h"
#include "IStatusDisplay.h"
#include "INotificationSound.h"

OrderEventHandler::OrderEventHandler(IStatusIndicator *indicator,
                                     IStatusDisplay *display,
                                     INotificationSound *buzzer)
    : _indicator(indicator), _display(display), _buzzer(buzzer) {}

void OrderEventHandler::handle(const OrderEvent &evt)
{
    Serial.printf("[Handler] Order #%d -> %s (eta %s)\n",
                  evt.order, evt.status, evt.eta);
    _display->update(evt.status, evt.order, evt.eta);
    _indicator->update(evt.status);
    _buzzer->playForStatus(evt.status);
}
