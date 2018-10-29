#ifndef CHECKSTATUSTHREAD_H
#define CHECKSTATUSTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "core_global.h"

class Instrument;

class CORE_EXPORT CheckStatusThread : public QThread
{
    Q_OBJECT

public:
    explicit CheckStatusThread(Instrument *parent = 0);
    ~CheckStatusThread();

    void start();
    void stop();
    void pause();

protected:
    virtual void run();

private:
    Instrument *m_instrument;

    volatile bool m_terminate;
    volatile bool m_pause;
    QMutex m_mutex;
    QWaitCondition m_waitCondition;
};

#endif // CHECKSTATUSTHREAD_H
