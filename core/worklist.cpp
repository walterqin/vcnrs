#include <QMutexLocker>
#include <QApplication>
#include <vcapp.h>

#include "global.h"
#include "worklist.h"
#include "numberrecognition.h"
#include "workbench.h"
#include "matlist.h"
#include "cameradata.h"
#include "instrument.h"

WorkList::WorkList(Workbench *workbench, QObject *parent)
    : QObject(parent)
    , m_workbench(workbench)
{
//   for (int i = 0; i < vc::CameraCnt; i++)
//        connect(m_instrList[i], SIGNAL(dataSampled(data)), this, SLOT(add(data)));
}

WorkList::~WorkList()
{
    qDeleteAll(m_matList);
}

void WorkList::setInstrument(Instrument *instr)
{
    if (m_instrList.contains(instr))
        return;
    m_instrList.append(instr);
}

void WorkList::setInstrumentes(QList<Instrument *> insList)
{
    m_instrList = insList;
}

void WorkList::setNumberRecognitionMachine(NumberRecognition *nrm)
{
    if (m_nrm)
        return;
    m_nrm = nrm;
}

void WorkList::insertSampleData(CameraData *data)
{
    emit dataAboutToBeInserted(data);
    {
        QMutexLocker locker(&m_mutex);
        m_matList.append(data);
        data->setWorklist(this);
    }
    emit dataInserted(data);
}

void WorkList::removeSampleData(CameraData *data)
{
    {
        QMutexLocker locker(&m_mutex);
        if (!m_matList.contains(data))
            return;
    }

    emit dataAboutToBeRemoved(data);

    {
        QMutexLocker locker(&m_mutex);
        if (m_matList.removeAll(data))
        {

        }
    }

    emit dataRemoved(data);
}
