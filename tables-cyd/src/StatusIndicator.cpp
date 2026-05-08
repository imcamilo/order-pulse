#include <Arduino.h>
#include "StatusIndicator.h"

StatusIndicator::StatusIndicator(int pinR, int pinG, int pinB)
    : _pinR(pinR), _pinG(pinG), _pinB(pinB) {}

void StatusIndicator::begin()
{
    pinMode(_pinR, OUTPUT);
    pinMode(_pinG, OUTPUT);
    pinMode(_pinB, OUTPUT);
    _set(false, false, false); // apagado al inicio
}

// r/g/b en true = encendido (escribe LOW por ánodo común)
void StatusIndicator::_set(bool r, bool g, bool b)
{
    digitalWrite(_pinR, r ? LOW : HIGH);
    digitalWrite(_pinG, g ? LOW : HIGH);
    digitalWrite(_pinB, b ? LOW : HIGH);
}

void StatusIndicator::update(const char *status)
{
    if (strcmp(status, "pending") == 0)
        _set(false, false, true);  // azul
    else if (strcmp(status, "preparing") == 0)
        _set(true, true, false);   // amarillo (R+G)
    else if (strcmp(status, "ready") == 0)
        _set(false, true, false);  // verde
    else
        _set(true, false, false);  // rojo (estado desconocido)
}
