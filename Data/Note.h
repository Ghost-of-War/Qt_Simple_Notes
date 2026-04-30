#ifndef NOTE_H
#define NOTE_H

#include <QString>
#include <QMetaType>
#include <qdatetime.h>

enum class Priority{
    JustInfo = 1,
    Low = 2,
    Medium = 3,
    High = 4,
    Urgent = 5
};

struct Note
{
    int id;
    QString title;
    QString text;
    QDateTime timestamp;
    Priority priority;
};

Q_DECLARE_METATYPE(Priority)
Q_DECLARE_METATYPE(Note)


#endif // NOTE_H
