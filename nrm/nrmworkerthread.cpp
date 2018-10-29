#include "nrmworkerthread.h"
#include "nrm.h"
#include "workbench.h"
#include "worklist.h"
#include "numberzone.h"

NRMWorkerThread::NRMWorkerThread(QObject *parent)
    : QObject(parent)
{
    m_nrm = 0;
    m_worklist = 0;

}

NRMWorkerThread::~NRMWorkerThread()
{
    stop();
}

void NRMWorkerThread::setOwner(NRM *nrm)
{
    if (m_nrm)
        return;
    m_nrm = nrm;
}

void NRMWorkerThread::setWorkList(WorkList *wk)
{
    if (m_worklist)
        return;
    m_worklist = wk;
}

void NRMWorkerThread::startThread()
{
    QMutexLocker locker(&m_mutex);
    m_abort = false;
}

void NRMWorkerThread::pause()
{
    QMutexLocker locker(&m_mutex);
    m_resume = false;
}

void NRMWorkerThread::stop()
{
    QMutexLocker locker(&m_mutex);
    m_abort = true;
}

void NRMWorkerThread::fetchCameraData(CameraData *mat)
{

}

void NRMWorkerThread::removeCameraData()
{

}

void NRMWorkerThread::process()
{
    {
        QMutexLocker locker(&m_mutex);
        m_resume = true;
        m_abort = false;

        if (!m_worklist)
            return;
    }

    while(1)
    {
        {
            QMutexLocker locker(&m_mutex);
            if (m_abort)
                return;

        }

        {
            QMutexLocker locker(m_worklist->mutex());
            foreach (CameraData *data, m_worklist->cameraData())
            {
                if (data->isValid())
                {
                    QVector<NumberZone> zone;
                    QStringList numStr;
                    if (m_nrm->numberRecognize(data->getCameraData(), zone) > 0 )
                    {
                        data->setValid(false);
                        foreach (NumberZone item, zone)
                            numStr << item.getNumberZoneStr();

                        emit recognizedResult(numStr);
                    }
                }
            }
        }
    }
}
