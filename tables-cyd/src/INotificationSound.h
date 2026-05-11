#ifndef I_NOTIFICATION_SOUND_H
#define I_NOTIFICATION_SOUND_H

class INotificationSound
{
public:
    virtual ~INotificationSound() = default;
    virtual void playForStatus(const char *status) = 0;
};

#endif
