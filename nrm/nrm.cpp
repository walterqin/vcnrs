
#include <QThread>

#include "nrm.h"
#include "nrmworkerthread.h"
#include "corefunc.h"
#include "zonedetect.h"
#include "zonejudge.h"
#include "zonelocate.h"
#include "character.h"
#include "charsidentify.h"
#include "charsrecognise.h"
#include "workbench.h"
#include "optionframe.h"
#include "nrmparampage.h"
#include "trainmodelpage.h"

using namespace corefunc;

NRM::NRM(Workbench *parent)
    : NumberRecognition(parent)
    , m_sim(false)
{
    m_workbench = parent;

    m_zoneDetect = new ZoneDetect(this);
    this->setZoneDetectInterface(m_zoneDetect);
    m_charsRecognise = new CharsRecognise(this);
    this->setCharsRecogniseInterface(m_charsRecognise);
}

NRM::~NRM()
{
    delete m_zoneDetect;
    delete m_charsRecognise;
}

void NRM::onStartAnalysis()
{
    if (!m_workerThread)
        startAnalyisThread();
    emit startWorkerThread();
}

void NRM::onStopAnalysis()
{
    if (m_workerThread)
    {
        if (m_worker)
            emit stopWorkerThread();
    }
}

void NRM::startAnalyisThread()
{
    if (m_workerThread)
        return;

    m_workerThread = new QThread;
    m_worker = new NRMWorkerThread();
    m_worker->setWorkList(m_workbench->workList());
    m_worker->moveToThread(m_workerThread);

    connect(m_workerThread, &QThread::finished, m_workerThread, &QObject::deleteLater);
    connect(m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(this, &NRM::startWorkerThread, m_worker, &NRMWorkerThread::process);
    connect(this, &NRM::stopWorkerThread, m_worker, &NRMWorkerThread::stop);
}

void NRM::addOptionPage(OptionFrame *frame)
{
    QTreeWidgetItem *parent = frame->addItem("识别算法");
    frame->addItem(new NrmParamPage(this), parent);
    frame->addItem(new TrainModelPage(this), parent);
}

void NRM::setDebug(bool flag)
{

}

void NRM::manualTest()
{

}

int NRM::numberRecognize(const Mat &src, QVector<NumberZone> &zoneOut, int img_index)
{
    return NumberRecognition::numberRecognize(src, zoneOut, img_index);
}

int NRM::numberRecognize(const Mat &src, QVector<NumberZone> &zoneOut)
{
    return numberRecognize(src, zoneOut, 0);
}

void NRM::setLifeMode(bool mode)
{

}

void NRM::setDetectType(int type)
{

}

void NRM::loadSVM(QString path)
{
    ZoneJudge::instance()->loadModel(path.toStdString());
}

void NRM::loadANN(QString path)
{
    CharsIdentify::instance()->LoadModel(path);
}

void NRM::loadChineseANN(QString path)
{
    CharsIdentify::instance()->LoadChineseModel(path);
}

void NRM::loadGrayChANN(QString path)
{
    CharsIdentify::instance()->LoadGrayChANN(path);
}

void NRM::loadChineseMapping(QString path)
{
    CharsIdentify::instance()->LoadChineseMapping(path);
}

void NRM::setSimulate(bool sim)
{
    if (m_sim == sim)
        return;
    m_sim = sim;
}
