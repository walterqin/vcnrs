/**
* @file workbench.cpp
* @brief 操作平台类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#include <QDebug>
#include <QToolBar>
#include <QSettings>
#include <QApplication>
#include <QHBoxLayout>
#include <QString>
#include <QFile>
#include <QScriptEngine>
#include <QInputDialog>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QBuffer>
#include <QTextStream>
#include <QStatusBar>
#include <QToolButton>
#include <QMenu>
#include <QMenuBar>
#include <QThread>

#include "global.h"
#include "workbench.h"
//#include "calibrationdoc.h"
//#include "calibrationmanager.h"
#include "mainwindowbase.h"
#include "mainwindow.h"
#include "vcactions.h"
#include "vcapp.h"
#include "messagewidget.h"
#include "mainpageframe.h"
#include "optionframe.h"
#include "simupageframe.h"
#include "logdataframe.h"
#include "calibrationframe.h"
#include "numberrecognition.h"
#include "numberrecognitionfactory.h"
#include "instrument.h"
#include "instrumentfactory.h"
#include "worklist.h"
#include "cameradata.h"
#include "taskframe.h"

typedef QList<QAction *> ActionList;


static inline void addActionsToMenu(QMenu *m, const ActionList &al)
{
    const ActionList::const_iterator cend = al.constEnd();
    for (ActionList::const_iterator it = al.constBegin(); it != cend; ++it)
        m->addAction(*it);
}

static inline QMenu *addMenu(QMenuBar *mb, const QString &title, const ActionList &al)
{
    QMenu *rc = mb->addMenu(title);
    addActionsToMenu(rc, al);
    return rc;
}

static void initGlobal()
{
//    qRegisterMetaType<AnalysisResult>();
    qRegisterMetaType<QMessageBox::StandardButton>();
    qRegisterMetaType<QMessageBox::StandardButton*>();
    qRegisterMetaType<QMessageBox::StandardButtons>();
    qRegisterMetaType<QMessageBox::Icon>();
    qRegisterMetaType< QList<int> >();
    qRegisterMetaType<Workbench*>();
}

Workbench::Workbench(const QVariantMap &instrMap, const QVariantMap &algorMap, QObject *parent)
    : QObject(parent)
    , d(new Data)
{
    initGlobal();

    connect(qApp, SIGNAL(userLogin(bool)), this, SIGNAL(userLogin(bool)));
    connect(qApp, SIGNAL(systemLogin()),   this, SIGNAL(systemLogin()));
    connect(qApp, SIGNAL(systemLogout()),  this, SIGNAL(systemLogout()));

    setTmpValue("instr", instrMap.value("name").toString());

    d->instrInfo = instrMap;
	d->algorInfo = algorMap;
    d->instrument = 0;
	d->algorithm = 0;
    d->mainWindow = 0;
    d->debugger = 0;
    d->actionManager = new VcActions(this);
    d->worklist = new WorkList(this);
	//    d->caliManager = qApp->calibrationManager();
//    d->curveManager = new StdCurveManager(this);
//    d->caliDoc = new CalibrationDoc(this);
//    d->printer = qApp->printer();

    initInstrument();
    if (!d->instrument)
        return;
	
	initAlgorithm();
	if (!d->algorithm)
		return;

    d->worklist->setInstrument(d->instrument);
    d->worklist->setNumberRecognitionMachine(d->algorithm);

    connect(d->instrument, SIGNAL(dataSampled(CameraData*)), d->worklist, SLOT(insertSampleData(CameraData*)));

    initMainWindow();

    d->actionManager->showMainPageAction->setChecked(true);
    connect(qApp, SIGNAL(initialized()), this, SLOT(showMainPage()));

    // 显示视图
    connect(d->actionManager->showMainPageAction, SIGNAL(triggered(bool)),
            this, SLOT(showMainPage()));
    connect(d->actionManager->showTaskAction, SIGNAL(triggered(bool)),
            this, SLOT(showTaskWindow()));
    connect(d->actionManager->showSimulateAction, SIGNAL(triggered(bool)),
            this, SLOT(showSimulation()));
    connect(d->actionManager->showCalibrateAction, SIGNAL(triggered(bool)),
            this, SLOT(showCalibration()));
    connect(d->actionManager->showLogAction, SIGNAL(triggered(bool)),
            this, SLOT(showLog()));
    connect(d->actionManager->showSystemAction, SIGNAL(triggered(bool)),
            this, SLOT(showSystem()));

    // 关于
    connect(d->actionManager->aboutAction, SIGNAL(triggered()), qApp, SLOT(about()));

    // 退出
    connect(d->actionManager->exitAction, SIGNAL(triggered()),
            d->mainWindow, SLOT(close()));

}

Workbench::~Workbench()
{
    qDelete(d->instrument);
	qDelete(d->algorithm);
    qDelete(d->worklist);
    if (d->mainWindow)
        delete d->mainWindow;
    if (d->debugger)
        delete d->debugger;
    if (d->actionManager)
        delete d->actionManager;
    delete d;
}

void Workbench::initInstrument()
{
    if (d->instrInfo.isEmpty())
        return;

    
    d->instrument = InstrumentFactory::create(d->instrInfo.value("dll").toString(), this);
    if (!d->instrument)
        return;

    d->debugger = d->instrument->initDebugger();

}

void Workbench::initAlgorithm()
{
	if (d->algorInfo.isEmpty())
		return;
	d->algorithm = NumberRecognitionFactory::create(d->algorInfo.value("dll").toString(), this);
	if (!d->algorithm)
		return;
}

void Workbench::initMainWindow()
{
    d->mainWindow = new MainWindow(this);
    d->mainWindow->setWindowTitle(qApp->applicationName());
    d->mainWindow->setAttribute(Qt::WA_DeleteOnClose);

	VcActions *actionManager = d->actionManager;

    QToolBar *toolBar = d->mainWindow->toolBar();
    toolBar->setWindowTitle(QString::fromLocal8Bit("标准"));
    toolBar->toggleViewAction()->setEnabled(false);
//    d->mainWindow->addToolBar(Qt::RightToolBarArea,toolBar);

    QWidget* w = new QWidget;
    w->setMinimumHeight(40);
    toolBar->addWidget(w);

    toolBar->addActions(actionManager->viewActions->actions());
    toolBar->addAction(actionManager->aboutAction);

    QWidget *widget = new QWidget;
    widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    toolBar->addWidget(widget);

    toolBar->addAction(actionManager->exitAction);

    d->messageWidget = d->mainWindow->messageWidget();
}

template<typename T>
T *Workbench::showWidget()
{
    T *w = qobject_cast<T*>(mainWindow()->centralWidget());
    if (!w)
    {
        if (mainWindow()->centralWidget())
            delete mainWindow()->centralWidget();

        w = new T(this, mainWindow());
        mainWindow()->setCentralWidget(w);
    }
    return w;
}

void Workbench::showMainPage()
{
    showWidget<MainPageFrame>();
}

void Workbench::showTaskWindow()
{
    showWidget<TaskFrame>();
}

void Workbench::showCalibration()
{
    showWidget<CalibrationFrame>();
}

void Workbench::showSimulation()
{
    showWidget<SimuPageFrame>();
}

void Workbench::showLog()
{
    showWidget<LogDataFrame>();
}

void Workbench::showSystem()
{
#if 0
    QDialog dlg(mainWindow());
    dlg.setWindowTitle(QString::fromLocal8Bit("系统设置"));
    dlg.setWindowModality(Qt::WindowModal);

    OptionFrame *frame = new OptionFrame(this, &dlg);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(frame);
    dlg.setLayout(layout);
    dlg.exec();
#else
    showWidget<OptionFrame>();
#endif
}

QVariant load_script(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        QString script = in.readAll();

        QScriptEngine engine;
        QVariant var = engine.evaluate(script, file.fileName()).toVariant();
        if (engine.hasUncaughtException())
        {
            critical(engine.uncaughtException().toString() + "\n" +
                     engine.uncaughtExceptionBacktrace().join("\n"));
            return QVariant();
        }
        file.close();  
		return var;
    }
    return QVariant();
}

QList<QVariantMap> load_instr_info()
{
    QString path  = qApp->applicationDirPath() + "/instr.js";
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString script = in.readAll();

    QScriptEngine engine;
    QVariant var = engine.evaluate(script, file.fileName()).toVariant();
    if (engine.hasUncaughtException())
    {
        critical(engine.uncaughtException().toString() + "\n" +
                 engine.uncaughtExceptionBacktrace().join("\n"));
        return QList<QVariantMap>();
    }

    QList<QVariant> list = var.toList();
    QList<QVariantMap> map;
    foreach(QVariant v, list)
    {
        QVariantMap vm = v.toMap();
        if (!vm.isEmpty())
             map.append(vm);
#ifndef QT_NO_DEBUG
        else
            qDebug() << "Can't change variant to QVariantMap: " << v;
#endif
    }

	if (map.isEmpty())
        critical(Workbench::tr(QString::fromLocal8Bit("无效的配置文件, 不包含任何仪器信息.").toStdString().data()));

    return map;
}

QVariantMap find_instr_info(const QList<QVariantMap> &map, const QString &name)
{
    foreach(QVariantMap m, map)
        if (m.value("name").toString() == name)
            return m;
    return QVariantMap();
}

void Workbench::showDebugger()
{
    if (d->debugger)
    {
        d->debugger->show();
        d->debugger->raise();
    }
}

bool Workbench::canClose()
{
    bool prompted = false;
//    if (d->caliManager && !d->caliManager->canClose(prompted))
//        return false;

//    if (d->caliDoc && !d->caliDoc->canClose(prompted))
//        return false;

    if (!prompted && question(QString::fromLocal8Bit("确认退出?")) == QMessageBox::No)
        return false;

    if (d->debugger)
        d->debugger->close();

    return true;
}

//bool Workbench::isSystemLogin() const
//{
//    return qApp->isSystemLogin();
//}

QVariant Workbench::value(const QString &key, const QVariant &defaultValue) const
{
    return qApp->value(key, defaultValue);
}

void Workbench::setValue(const QString &key, const QVariant &value)
{
    qApp->setValue(key, value);
}

QVariant Workbench::tmpValue(const QString &key, const QVariant &defaultValue) const
{
    QReadLocker locker(&m_readWriteLock);
    return m_tmp.value(key, defaultValue);
}

void Workbench::setTmpValue(const QString &key, const QVariant &value)
{
    QWriteLocker locker(&m_readWriteLock);
    m_tmp[key] = value;
}

QWidget *Workbench::safeParentWindow() const
{
    QWidget *parent = qApp->activeModalWidget();
    if (!parent)
        parent = d->mainWindow;
    if (parent && parent->window())
        parent = parent->window();

    return parent;
}

void Workbench::showMessageBox(QMessageBox::StandardButton *response,
                               QMessageBox::Icon icon,
                               const QString &text,
                               QMessageBox::StandardButtons buttons,
                               QMessageBox::StandardButton defaultButton)
{
    WaitCursor arrow(Qt::ArrowCursor);
    QWidget *parent = safeParentWindow();
    QMessageBox box(icon,
                    d->mainWindow ? d->mainWindow->windowTitle()
                                  : qApp->applicationName(),
                    text,
                    buttons,
                    parent);

    if (defaultButton != QMessageBox::NoButton)
        box.setDefaultButton(defaultButton);

    *response = (QMessageBox::StandardButton)box.exec();
}

QMessageBox::StandardButton
Workbench::information(const QString &text,
                       QMessageBox::StandardButtons buttons,
                       QMessageBox::StandardButton defaultButton)
{
    QMessageBox::StandardButton button = QMessageBox::NoButton;
    Qt::ConnectionType type = QThread::currentThread() == thread()
            ? Qt::DirectConnection : Qt::BlockingQueuedConnection;
    QMetaObject::invokeMethod(this,  "showMessageBox", type,
                              Q_ARG(QMessageBox::StandardButton *, &button),
                              Q_ARG(QMessageBox::Icon, QMessageBox::Information),
                              Q_ARG(QString, text),
                              Q_ARG(QMessageBox::StandardButtons, buttons),
                              Q_ARG(QMessageBox::StandardButton, defaultButton));
    return button;
}

QMessageBox::StandardButton
Workbench::question(const QString &text,
                    QMessageBox::StandardButtons buttons,
                    QMessageBox::StandardButton defaultButton)
{
    QMessageBox::StandardButton button = QMessageBox::NoButton;
    Qt::ConnectionType type = QThread::currentThread() == thread()
            ? Qt::DirectConnection : Qt::BlockingQueuedConnection;
    QMetaObject::invokeMethod(this, "showMessageBox", type,
                              //Q_RETURN_ARG(QMessageBox::StandardButton, button),
                              Q_ARG(QMessageBox::StandardButton *, &button),
                              Q_ARG(QMessageBox::Icon, QMessageBox::Question),
                              Q_ARG(QString, text),
                              Q_ARG(QMessageBox::StandardButtons, buttons),
                              Q_ARG(QMessageBox::StandardButton, defaultButton));
    return button;
}

QMessageBox::StandardButton
Workbench::warning(const QString &text,
                   QMessageBox::StandardButtons buttons,
                   QMessageBox::StandardButton defaultButton)
{
    QMessageBox::StandardButton button = QMessageBox::NoButton;
    Qt::ConnectionType type = QThread::currentThread() == thread()
            ? Qt::DirectConnection : Qt::BlockingQueuedConnection;
    QMetaObject::invokeMethod(this, "showMessageBox", type,
                              //Q_RETURN_ARG(QMessageBox::StandardButton, button),
                              Q_ARG(QMessageBox::StandardButton *, &button),
                              Q_ARG(QMessageBox::Icon, QMessageBox::Warning),
                              Q_ARG(QString, text),
                              Q_ARG(QMessageBox::StandardButtons, buttons),
                              Q_ARG(QMessageBox::StandardButton, defaultButton));
    return button;
}

QMessageBox::StandardButton
Workbench::critical(const QString &text,
                    QMessageBox::StandardButtons buttons,
                    QMessageBox::StandardButton defaultButton)
{
    QMessageBox::StandardButton button = QMessageBox::NoButton;
    Qt::ConnectionType type = QThread::currentThread() == thread()
            ? Qt::DirectConnection : Qt::BlockingQueuedConnection;
    QMetaObject::invokeMethod(this, "showMessageBox", type,
                              //Q_RETURN_ARG(QMessageBox::StandardButton, button),
                              Q_ARG(QMessageBox::StandardButton *, &button),
                              Q_ARG(QMessageBox::Icon, QMessageBox::Critical),
                              Q_ARG(QString, text),
                              Q_ARG(QMessageBox::StandardButtons, buttons),
                              Q_ARG(QMessageBox::StandardButton, defaultButton));
    return button;
}

void Workbench::showInformation(const QString &text, bool show)
{
    showMessage(QtDebugMsg, text, show);
}

void Workbench::showWarning(const QString &text, bool show)
{
    showMessage(QtWarningMsg, text, show);
}

void Workbench::showCritical(const QString &text, bool show)
{
    showMessage(QtCriticalMsg, text, show);
}

void Workbench::showFatal(const QString &text, bool show)
{
    showMessage(QtFatalMsg, text, show);
}

void Workbench::showMessage(int type, const QString &text, bool show)
{
    showMessage(0, type, text, show);
}

void Workbench::showMessage(int channel, int type, const QString &text, bool show)
{
    QMetaObject::invokeMethod(
                d->messageWidget,
                "showMessage",
                Q_ARG(Workbench*, this),
                Q_ARG(int, channel),
                Q_ARG(int, type),
                Q_ARG(QString, text),
                Q_ARG(bool, show));
}

void Workbench::showStatusMessage(const QString &text, int timeout)
{
    QMetaObject::invokeMethod(
                d->mainWindow->statusBar(),
                "showMessage",
                Q_ARG(QString, text),
                Q_ARG(int, timeout));
}
