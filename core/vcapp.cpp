#include <Windows.h>
#include <conio.h>
#include <QSettings>
#include <QReadWriteLock>
#include <QThread>
#include <QtConcurrentRun>
#include <QLibraryInfo>
#include <QSysInfo>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <QString>
#include <QStringList>
#include <QTimer>

#include "global.h"
#include "workbench.h"
#include "mainwindow.h"
#include "vcactions.h"
//#include "calibrationmanager.h"
#include "vcapp.h"
#include "mainwindow.h"

#define VERSION_STR "0.0.0.1"

void usage()
{
    const char *str =
            "cv [options...]\n"
            "\n"
            "-h               显示本信息\n"
            "-instr <type>    仪器类型\n"
            "-port <ports>    连接端口, 如果需要多个端口, 可用逗号(,)隔开\n"
            "-sim             模拟运行, 忽略连接端口\n"
            ;
    information(str);
}

static bool loadTranslator(QTranslator *translator, const QStringList &dir, const QString &fileName)
{
    foreach (const QString &path, dir)
        if (translator->load(fileName, path))
            return true;
    return false;
}

static void (*fnBeep) (int , int);

static void beepLoop(int freq, int duration, int sleep, int repeat)
{
    while (repeat-- > 0) {
        fnBeep(freq, duration);
        Sleep(sleep);
    }
}

static void beep_winnt(int freq, int duration)
{
    Beep(freq, duration);
}

//////////////////////////////////////////////////////////////////
/// \brief The PlaySoundThread class
///
class PlaySoundThread : public QThread
{
public:
    PlaySoundThread(QObject *parent = 0) : QThread(parent) {}

    void start(int freq, int duration, int interval)
    {
        m_run = true;
        m_freq = freq;
        m_duration = duration;
        m_interval = interval;

        if (!isRunning())
            QThread::start();
    }

    void stop()
    {
        m_run = false;
    }

protected:
    virtual void run()
    {
        while(m_run)
        {
            VcApp::beep(m_freq, m_duration);
            msleep(m_interval);
        }
    }

    volatile bool m_run;
    int m_freq;
    int m_duration;
    int m_interval;
};

//////////////////////////////////////////////////////////////////
/// VcApp class
///
VcApp::VcApp(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_playThread(new PlaySoundThread(this))
    , m_workDir(QDir::currentPath())
    , m_readWriteLock(new QReadWriteLock(QReadWriteLock::Recursive))
    , m_icons(0)
    , m_mainWindow(0)
    , m_settings(new QSettings("settings.ini", QSettings::IniFormat, this))
{
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
//    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    fnBeep = beep_winnt;
    if (!initialize())
        QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
}

VcApp::~VcApp()
{
    m_playThread->stop();
    m_playThread->wait();

    delete m_workbenches;
    delete m_settings;
//    delete m_calibrationManager;
    delete m_icons;
    delete m_readWriteLock;
}

bool VcApp::initialize()
{
    // 安装翻译词典
    QStringList searchDir;
    searchDir << "." << ".." << QLibraryInfo::location(QLibraryInfo::TranslationsPath);

    QSettings settings("settings.ini", QSettings::IniFormat);
    QStringList translations = settings.value("settings/translations", "qt_zh_cn").toStringList();
    foreach(QString translatorFileName, translations)
    {
        QTranslator *qtTranslator = new QTranslator(this);
        loadTranslator(qtTranslator, searchDir, translatorFileName);
        installTranslator(qtTranslator);
    }

	QString instrName = settings.value("instr", "dalsag3gc11").toString();
	QString algorName = settings.value("algor", "nrm").toString();

    setOrganizationName(QString::fromLocal8Bit("北京东方瑞威科技发展股份有限公司"));
    setApplicationName(QString::fromLocal8Bit("集装箱箱号识别系统"));
    setApplicationVersion(VERSION_STR);

    setTmpValue("debug/sim", true);

    QStringList args = QApplication::arguments();
    for (int i = 0; i < args.size(); i++)
    {
        if (args[i] == "-h")
        {
            usage();
            return false;
        }
        else if (args[i] == "-instr")
        {
            if (++i < args.size())
                setTmpValue("instr", args[i]);
        }
        else if (args[i] == "-port")
        {
            if (++i < args.size())
                setTmpValue("port", args[i]);
        }
		else if (args[i] == "-algor")
		{
			if (++i < args.size())
				setTmpValue("algor", args[i]);
		}
        else if (args[i] == "-sim")
            setTmpValue("debug/sim", true);
        else if (args[i] == "-stat")
            setTmpValue("debug/statCurve", true);
    }

    // 登录
    if (!login())
        return false;

    // 加载配置文件
    QList<QVariantMap> map = load_instr_info();
    if (map.isEmpty())
        return false;

    QVariantMap instrInfo = find_instr_info(map, instrName);
    if (instrInfo.isEmpty())
    {
        information(QString::fromLocal8Bit("找不到指定的仪器类型 %1").arg(instrName));
        return false;
    }
	QVariantMap algorInfo = find_instr_info(map, algorName);
	if (algorInfo.isEmpty())
	{
		information(QString::fromLocal8Bit("找不到指定的算法插件 %1").arg(algorName));
		return false;
	}

    // 系统资源
    m_icons = new VcIcons;
//    m_calibrationManager = new CalibrationManager(this);
    m_workbenches = new Workbench(instrInfo, algorInfo, this);

    // 显示主窗口
    if (!m_mainWindow)
        m_mainWindow = m_workbenches->mainWindow();
    m_mainWindow->showMaximized();

    emit initialized();

	return true;
}

void VcApp::beep(int freq, int duration)
{
    fnBeep(freq, duration);
}

void VcApp::playSound(int freq, int duration)
{
    QtConcurrent::run(fnBeep, freq, duration);
}

void VcApp::playSound(int freq, int duration, int sleep, int repeat)
{
    QtConcurrent::run(beepLoop, freq, duration, sleep, repeat);
}

void VcApp::playSound(VcApp::SoundType type)
{
    m_playThread->start(1000, 800, 200);
}

void VcApp::stopSound()
{
    m_playThread->stop();
}

QVariant VcApp::value(const QString &key, const QVariant &defaultValue) const
{
    QReadLocker locker(m_readWriteLock);
    return m_settings->value(key, defaultValue);
}

void VcApp::setValue(const QString &key, const QVariant &value)
{
    QWriteLocker locker(m_readWriteLock);
    if (value.isValid())
        m_settings->setValue(key, value);
    else
        m_settings->remove(key);
}

QVariant VcApp::tmpValue(const QString &key, const QVariant &defaultValue) const
{
    QReadLocker locker(m_readWriteLock);
    return m_tmp.value(key, defaultValue);
}

void VcApp::setTmpValue(const QString &key, const QVariant &value)
{
    QWriteLocker locker(m_readWriteLock);
    if (value.isValid())
        m_tmp[key] = value;
    else
        m_tmp.remove(key);
}

QIcon VcApp::icon(const QString &name)
{
    return m_icons->icon(name);
}

bool VcApp::isSystemLogin() const
{
    return tmpValue("user/level").toInt() > Level10;
}

bool VcApp::login()
{
    return true;
}

void VcApp::about()
{
    QString text = "<html>\n"
            "<style type=\"text/css\">\n"
            "body	{ font-family: Sans-serif }\n"
            "table	{ margin-left: 9px; margin-right: 9px }\n"
            "td		{ margin: 3px }\n"
            ".key	{ padding-right: 6px }\n"
            ".value { padding-right: 16px }\n"
            "</style>\n";

    text += "<body><h2>" + qApp->applicationName() + "</h2>\n";
    text += QString::fromLocal8Bit("版本: ") + qApp->applicationVersion() + "<br/>\n";

//    foreach(Workbench *w, workbenches())
//    {
//        text += "<br/>\n";
//        QString title = w->instrInfo("title").toString();
//        if (title.isEmpty())
//            title = w->instrInfo("name").toString();
//        text += title;
//        text += "<hr>";
//        text += w->about();
//    }

    text += "</body></html>";

    QMessageBox::about(m_mainWindow, QString::fromLocal8Bit("关于"), text);
}

