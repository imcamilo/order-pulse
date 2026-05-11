#ifndef I_STATUS_DISPLAY_H
#define I_STATUS_DISPLAY_H

class IStatusDisplay
{
public:
    virtual ~IStatusDisplay() = default;
    virtual void update(const char *status, int orderId, const char *eta) = 0;
};

#endif
