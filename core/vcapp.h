#ifndef VCAPP_H
#define VCAPP_H

#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>
#include <QList>
#include <QDir>
#include <QMap>
#include <QVariant>

#include "core_global.h"

#undef qApp
#define qApp (static_cast<VcApp*>(QCoreApplication::instance()))

class Workbench;
class PlaySoundThread;
class QSettings;
class QReadWriteLock;
class QMainWindow;
class VcIcons;
class CalibrationManager;


class CORE_EXPORT VcApp : public QApplication
{
    Q_OBJECT
public:
    enum SoundType {
        General,		// 通用
        SensorExp,		// 传感器异常
    };

    enum UserLevel
    {
        Level1,
        Level2,
        Level3,
        Level4,
        Level5,

        Level10 = 10,
    };

    VcApp(int &argc, char **argv);
    ~VcApp();

    Workbench *workbenches() const { return m_workbenches; }
    QDir workdir() const { return m_workDir; }

    bool initialize();

    static void beep(int freq, int duration);
    void playSound(int freq, int duration);
    void playSound(int freq, int duration, int sleep, int repeat);
    void playSound(SoundType type = General);
    void stopSound();

    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
    template<class T> T value(const QString &key) const { return value(key).value<T>(); }

    QVariant tmpValue(const QString &key, const QVariant &defaultValue=QVariant()) const;
    void setTmpValue(const QString &key, const QVariant &value);
    template<class T> T tmpValue(const QString &key) const { return tmpValue(key).value<T>(); }

    VcIcons *icons() const { return m_icons; }
    QIcon icon(const QString &name);

    bool isSystemLogin() const;
    CalibrationManager *calibrationManager() const { return m_calibrationManager; }

public slots:
    bool login();
    void about();

signals:
    void initialized();
    void userLogin(bool system);
    void systemLogin();
    void systemLogout();

private:
    void createMainWindow();

private:
    Workbench *m_workbenches;
    PlaySoundThread *m_playThread;
    QDir m_workDir;
    QSettings *m_settings;
    QMap<QString, QVariant> m_tmp; //变量临时存储
    mutable QReadWriteLock *m_readWriteLock;
    VcIcons *m_icons;
    CalibrationManager *m_calibrationManager;

    QMainWindow *m_mainWindow;
};

#endif // VCAPP_H
