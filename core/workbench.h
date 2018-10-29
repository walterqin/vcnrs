/**
* @file workbench.h
* @brief 轨道衡仪器操作平台类定义，包括采集、测试、标定及质控等
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef WORKBENCH_H
#define WORKBENCH_H

#include <QObject>
#include <QPointer>
#include <QVariant>
#include <QMap>
#include <QMessageBox>
#include <QReadWriteLock>

#include "core_global.h"
#include "mainwindow.h"

class VcActions;
//class CalibrationManager;
class MessageWidget;
class QSettings;
class QAction;
class QMenu;
class QDeclarativeView;
class NumberRecognition;
class Instrument;
class WorkList;

class CORE_EXPORT Workbench : public QObject
{
    Q_OBJECT
public:
    explicit Workbench(const QVariantMap &instrMap, const QVariantMap &algorMap, QObject *parent = 0);
    ~Workbench();

    VcActions *actionManager() const { return d->actionManager; }
    MainWindow *mainWindow() const { return d->mainWindow; }
//    CalibrationManager *calibrationManager() const { return d->caliManager; }

    QVariant value(const QString &key, const QVariant &defaultValue=QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
    template<class T> T value(const QString &key) const { return value(key).value<T>(); }

    QVariant tmpValue(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setTmpValue(const QString &key, const QVariant &value);
    template<class T> T tmpValue(const QString &key) const { return tmpValue(key).value<T>(); }

    QVariantMap instrInfo() const { return d->instrInfo; }
    Instrument *instrument() const { return d->instrument; }
    NumberRecognition *nrm() const { return d->algorithm; }
    WorkList *workList() const { return d->worklist; }
//    DataStorage *printer() const;
    NumberRecognition *algorithm() const { return d->algorithm; }

    bool canClose();

    QWidget *safeParentWindow() const;

    QMessageBox::StandardButton
    information(const QString &text,
                QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

    QMessageBox::StandardButton
    question(const QString &text,
             QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No,
             QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

    QMessageBox::StandardButton
    warning(const QString &text,
            QMessageBox::StandardButtons buttons = QMessageBox::Ok,
            QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

    QMessageBox::StandardButton
    critical(const QString &text,
             QMessageBox::StandardButtons buttons = QMessageBox::Ok,
             QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

signals:
    void userLogin(bool system);
    void systemLogin();
    void systemLogout();

public slots:
    void showMainPage();
    void showTaskWindow();
    void showCalibration();
    void showSimulation();
    void showLog();
    void showSystem();
    void showDebugger();
    void showInformation(const QString &text, bool show = true);
    void showWarning(const QString &text, bool show = true);
    void showCritical(const QString &text, bool show = true);
    void showFatal(const QString &text, bool show = true);
    void showMessage(int channel, int type, const QString &text, bool show = true);
    void showMessage(int type, const QString &text, bool show = true);

    void showMessageBox(QMessageBox::StandardButton *response,
                        QMessageBox::Icon icon,
                        const QString &text,
                        QMessageBox::StandardButtons buttons,
                        QMessageBox::StandardButton defaultButton);
    void showStatusMessage(const QString &text, int timeout = 2000);


private:
    void initInstrument();
	void initAlgorithm();
    void initMainWindow();

    template<typename T>  T *showWidget();

    struct Data{
        VcActions				*actionManager;
//        CalibrationManager		*caliManager;
//        CalibrationDoc			*caliDoc;
//        DataStorage				*printer;
        NumberRecognition       *algorithm;
        Instrument              *instrument;
        QPointer<QWidget>		debugger;
        QPointer<MainWindow>    mainWindow;
        QVariantMap				instrInfo;
		QVariantMap				algorInfo;
        QPointer<MessageWidget> messageWidget;
        WorkList                *worklist;
    };

    Data *d;

    QMap<QString, QVariant> m_tmp;
    mutable QReadWriteLock m_readWriteLock;
};

QVariant load_script(const QString &fileName);
QList<QVariantMap> load_instr_info();
QVariantMap find_instr_info(const QList<QVariantMap> &map, const QString &name);

Q_DECLARE_METATYPE(QMessageBox::StandardButton)
Q_DECLARE_METATYPE(QMessageBox::StandardButton*)
Q_DECLARE_METATYPE(QMessageBox::StandardButtons)
Q_DECLARE_METATYPE(QMessageBox::Icon)
Q_DECLARE_METATYPE(QList<int>)
Q_DECLARE_METATYPE(Workbench*)

#endif // WORKBENCH_H
