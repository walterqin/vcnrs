#ifndef VCACTIONS_H
#define VCACTIONS_H

#include <QObject>
#include <QIcon>
#include <QMap>

class Workbench;
class QActionGroup;
class QAction;
class QKeySequence;

class VcIcons
{
public:
    VcIcons();
    QIcon icon(const QString &name);
    QIcon operator[](const QString &name) { return icon(name); }

private:
    QMap<QString, QIcon> icons;
    QString path;
};

class VcActions : public QObject
{
    Q_OBJECT
public:
    explicit VcActions(Workbench *parent = 0);
    ~VcActions();

    VcIcons &icons;

    QActionGroup *viewActions;
    QActionGroup *mainActions;
    QActionGroup *taskActions;
    QActionGroup *simulateActions;
    QActionGroup *calibrateActions;
    QActionGroup *logActions;
    QActionGroup *systemActions;
    QActionGroup *helpActions;

    /* 菜单显示 */
    QAction *showMainPageAction;
    QAction *showTaskAction;
    QAction *showSimulateAction;
    QAction *showCalibrateAction;
    QAction *showLogAction;
    QAction *showSystemAction;
    QAction *showHelpAction;

    /* 主界面 */
    QAction *main3DviewAction;
    QAction *mainComplateAction;
    QAction *mainCancelAction;
    QAction *mainPLCAction;

    /* 信息 */
    QAction *taskAccepteAction;
    QAction *taskDeleteAction;
    QAction *taskQueryAction;
    QAction *taskDetailAction;
    QAction *taskPrintAction;

    /* 仿真 */
    QAction *simuForwardAction;
    QAction *simuBackwardAction;
    QAction *simulateStartAction;
    QAction *simulateStopAction;
    QAction *simulateMapAction;
    QAction *simulateModeAction;
    QAction *simulateMLAction;

    /* 标定 */
    QAction *calibrateStartAction;
    QAction *calibrateStopAction;
    QAction *calibrateOpenAction;
    QAction *calibrateVideoAction;
    QAction *calibrateSaveAction;
    QAction *calibrateDetectAction;
    QAction *calibrateAddImageAction;
    QAction *calibrateRemoveImageAction;
    QAction *calibrateSettingAction;

    /* 日志 */
    QAction *logAllDataAction;
    QAction *logRunningAction;
    QAction *logSystemAction;

    /* 系统维护 */
    QAction *systemConfigAction;    //系统配置
    QAction *systemParamAction;     //参数设置
    QAction *systemAddUserAction;   //添加用户
    QAction *systemMaintainAction;  //硬件维护

    QAction *aboutAction;     // 帮助
    QAction *exitAction;      // 退出

signals:

public slots:

private:
    Workbench *m_workbench;
    QAction *createAction(const QString &iconName, const QString &text);
    QAction *createAction(const QString &iconName, const QString &text, const QKeySequence &shortcut);

};

#endif // VCACTIONS_H
