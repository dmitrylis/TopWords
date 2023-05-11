#ifndef TWTYPES_H
#define TWTYPES_H

#include <QObject>

class TwTypes final
{
    Q_GADGET

public:
    enum ReadingStatus
    {
        Idle = 0,
        InProgress,
        Paused,
        Finished
    };
    Q_ENUM(ReadingStatus)
};

#endif // TWTYPES_H
