#ifndef STATUS_INDICATOR_H
#define STATUS_INDICATOR_H

// LED RGB discreto de la CYD (ánodo común — LOW enciende, HIGH apaga).
// Mapea estados a colores: pending=azul, preparing=amarillo, ready=verde.
class StatusIndicator
{
private:
    int _pinR, _pinG, _pinB;
    void _set(bool r, bool g, bool b);

public:
    StatusIndicator(int pinR, int pinG, int pinB);
    void begin();
    void update(const char *status);
};

#endif
