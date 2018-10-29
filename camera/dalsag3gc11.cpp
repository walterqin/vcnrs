#include <QVarLengthArray>
#include <QtEndian>
#include <QLabel>
#include <QBoxLayout>
#include <QStatusBar>
#include <QTextCodec>
#include <QLocale>
#include <QString>
#include <QTranslator>

#include "dalsag3gc11.h"
#include "workbench.h"
#include "vcapp.h"
#include "profile.h"
#include "optionframe.h"
#include "connectionpage.h"
#include "parampage.h"
#include "debugpage.h"
#include "workerthreaddalsag3gc11.h"
#include "global.h"
#include "instrument.h"

DalsaG3GC11::DalsaG3GC11(Workbench *parent)
    : Instrument(parent)
    , m_commMutex(QMutex::Recursive)
{
    m_workerThread = 0;
    m_worker = 0;

    std::memset(&m_instrParam, 0, sizeof(m_instrParam));
}

DalsaG3GC11::~DalsaG3GC11()
{
    disconnectFromHost();
}

//QVariant DalsaG3GC11::instrumentInfo(Info info) const
//{

//}

QString DalsaG3GC11::about() const
{
    return tr("Dalsa G3-GC11-C2420");
}

void DalsaG3GC11::connectToHost()
{

}

void DalsaG3GC11::disconnectFromHost()
{

}

void DalsaG3GC11::waitForConnected()
{

}

QIODevice *DalsaG3GC11::openParamFile(QIODevice::OpenMode mode)
{
	return 0;
}

void DalsaG3GC11::addOptionPage(OptionFrame *frame)
{
    QTreeWidgetItem *parent = frame->addItem(QString("相机设置"));
    frame->addItem(new ConnectionPage(this), parent);
    frame->addItem(new ParamPage(this), parent);
    frame->addItem(new DebugPage(this), parent);

}

void DalsaG3GC11::initMainWindow(QMainWindow *window)
{

}

void DalsaG3GC11::manualTest()
{

}

void DalsaG3GC11::updateStatus()
{

}

void DalsaG3GC11::onOpenCamera()
{

}

void DalsaG3GC11::onCloseCamera()
{

}

void DalsaG3GC11::start()
{
    if (!m_workerThread)
        imageAcquisitionProcess();

    emit acquisitionStart();
}

void DalsaG3GC11::pause()
{
    if (m_workerThread)
    {
        if (m_worker)
            emit acquisitionPause();
    }
}

void DalsaG3GC11::end()
{
    if (m_workerThread)
    {
        if (m_worker)
            emit acquisitionStop();
    }
}

bool DalsaG3GC11::reset()
{
	return true;
}

void DalsaG3GC11::clean()
{

}

void DalsaG3GC11::imageAcquisitionProcess()
{
    if (m_workerThread)
        return;

    m_workerThread = new QThread;
    m_worker = new WorkerThreadDalsaG3GC11();
    m_worker->setOwner(this);
    m_worker->moveToThread(m_workerThread);

    connect(m_workerThread, &QThread::finished, m_workerThread, &QObject::deleteLater);
    connect(m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(this, &DalsaG3GC11::acquisitionStart, m_worker, &WorkerThreadDalsaG3GC11::start);
    connect(this, &DalsaG3GC11::acquisitionPause, m_worker, &WorkerThreadDalsaG3GC11::pause);
    connect(this, &DalsaG3GC11::acquisitionStop, m_worker, &WorkerThreadDalsaG3GC11::stop);
}

