#include <QElapsedTimer>

#include "checkstatusthread.h"
#include "instrument.h"

CheckStatusThread::CheckStatusThread(Instrument *parent)
    : QThread(parent)
{
    m_instrument = parent;
    m_terminate = false;
    m_pause = false;
}

CheckStatusThread::~CheckStatusThread()
{
    stop();
}

void CheckStatusThread::start()
{
    m_pause = false;
    QThread::start();
}

void CheckStatusThread::stop()
{
    {
        QMutexLocker locker(&m_mutex);
        m_terminate = true;
        m_waitCondition.wakeAll();
    }
    wait();
}

void CheckStatusThread::pause()
{
    m_pause = true;
}

void CheckStatusThread::run()
{
    m_terminate = false;
    m_pause = false;

    while (!m_terminate)
    {
        if (m_pause)
        {
            sleep(10);
            continue;
        }

        if (m_instrument->instantUpdate())
        {
            QElapsedTimer timer;
            timer.start();
            while(!timer.hasExpired(500))
            {
                m_instrument->instantUpdate();
                sleep(1);
            }
            continue;
        }

        QMutexLocker locker(&m_mutex);
        if (m_waitCondition.wait(&m_mutex, 500))
            break;
    }
}
