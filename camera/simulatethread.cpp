#include <QFile>
#include <QDir>
#include <QTimer>

#include "simulatethread.h"
#include "cameradata.h"
#include "worklist.h"
#include "global.h"
#include "cameradata.h"
#include "worklist.h"
#include "workbench.h"


SimulateThread::SimulateThread(QObject *parent)
    : QObject(parent)
{
    m_instr = 0;
    m_param.clear();

    QDir dir("d:\\sim\\");
    m_simFileList = dir.entryInfoList("*.png *.jpg *.bmp");

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(process()));
}

void SimulateThread::setOwner(Instrument *instr)
{
    if (m_instr)
        return;
    m_instr = instr;
}

void SimulateThread::setSimulateParem(QMap<QString, QString> &param)
{
    if (!m_param.isEmpty())
        return;
    m_param = param;
}

void SimulateThread::setWorkList(WorkList *w)
{
    if (m_worklist)
        return;
    m_worklist = w;
}

void SimulateThread::process()
{
    if (!m_worklist)
        return;

    QString path = m_param["path"];
    QString delay = m_parma["delay"];


    QDir simDir(path);
    foreach(QString &fn, simDir.entryList("*.png *.jpg *.bmp"))
    {
        cv::Mat mat = imread(fn);
        CameraData data(mat);

        m_worklist->insertSampleData(&data);
        emit fetchDataCompletement();

    }
}
