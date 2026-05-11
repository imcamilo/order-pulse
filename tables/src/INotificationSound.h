#ifndef I_NOTIFICATION_SOUND_H
#define I_NOTIFICATION_SOUND_H

// Notification sound abstraction. Implemented by NotificationSound
// (hardware, LEDC-driven) and by MockNotificationSound in tests.
class INotificationSound
{
public:
    virtual ~INotificationSound() = default;
    virtual void playOrderReady() = 0;
};

#endif
