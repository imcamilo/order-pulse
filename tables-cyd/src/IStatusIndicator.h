#ifndef I_STATUS_INDICATOR_H
#define I_STATUS_INDICATOR_H

class IStatusIndicator
{
public:
    virtual ~IStatusIndicator() = default;
    virtual void update(const char *status) = 0;
};

#endif
