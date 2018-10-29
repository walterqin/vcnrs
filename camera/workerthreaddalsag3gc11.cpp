#include "workerthreaddalsag3gc11.h"
#include "dalsag3gc11.h"
#include "instrument.h"
#include "worklist.h"
#include "workbench.h"
#include "cameradata.h"
#include "numberrecognition.h"

WorkerThreadDalsaG3GC11::WorkerThreadDalsaG3GC11(QObject *parent)
    : QObject(parent)
{
    m_instrument = 0;
    m_resume = true;
    m_stop = false;
    m_running = false;
}

WorkerThreadDalsaG3GC11::~WorkerThreadDalsaG3GC11()
{
    stop();
}

void WorkerThreadDalsaG3GC11::setOwner(Instrument *instr)
{
    if (m_instrument)
        return;
    m_instrument = reinterpret_cast<DalsaG3GC11 *>(instr);
}

bool WorkerThreadDalsaG3GC11::isRunning()
{
    return m_running;
}

void WorkerThreadDalsaG3GC11::start()
{
    if (isRunning())
        return;

    QMutexLocker locker(&m_mutex);
    m_resume = true;
    m_stop = false;
}

void WorkerThreadDalsaG3GC11::pause()
{
    QMutexLocker locker(&m_mutex);
    m_resume = false;
}

void WorkerThreadDalsaG3GC11::stop()
{
    QMutexLocker locker(&m_mutex);
    m_stop = true;
    m_running = false;
}

void WorkerThreadDalsaG3GC11::process()
{
    {
        QMutexLocker locker(&m_mutex);
        m_resume = true;
        m_stop = false;
        m_running = true;
    }

    while(1)
    {
        {
            QMutexLocker locker(&m_mutex);
            if (m_stop)
            {
                m_resume = false;
                m_running = false;
                return;
            }
        }

        {
            emit dataSampledCompletement();
        }
    }
}
