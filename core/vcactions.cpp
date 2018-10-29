#include <QActionGroup>
#include <QMenu>
#include <QIcon>
#include <QString>
#include <QTextCodec>

#include "vcactions.h"
#include "workbench.h"
#include "vcapp.h"


static QAction *createSeparator(QObject *parent)
{
    QAction * rc = new QAction(parent);
    rc->setSeparator(true);
    return rc;
}

static QActionGroup *createActionGroup(QObject *parent, bool exclusive = false)
{
    QActionGroup * rc = new QActionGroup(parent);
    rc->setExclusive(exclusive);
    return rc;
}

//////////////////////////////////////////////////////////////////
/// WBIcons类

/**
* @brief 构造函数
*/
VcIcons::VcIcons()
    : path(":/icons/")
{

}

/**
* @brief 获取图标
* @param [I] 图标名称
* @retval 图标类
*/
QIcon VcIcons::icon(const QString &name)
{
    QMap<QString, QIcon>::iterator it = icons.find(name);
    if (it != icons.end())
        return *it;

    QIcon i(path + name + ".png");
    icons[name] = i;
    return i;
}

////////////////////////////////////////////////////////////
/// \brief VcActions class
///
VcActions::VcActions(Workbench *parent)
    : QObject(parent)
    , m_workbench(parent)
    , icons(*qApp->icons())

    , viewActions(createActionGroup(this))

    // 功能分组
    , mainActions(createActionGroup(this))
    , taskActions(createActionGroup(this))
    , simulateActions(createActionGroup(this))
    , calibrateActions(createActionGroup(this))
    , logActions(createActionGroup(this))
    , systemActions(createActionGroup(this))
    , helpActions(createActionGroup(this))

    // 视图
    , showMainPageAction(new QAction(icons["camera"], QString::fromLocal8Bit("监控"), this))
    , showTaskAction(new QAction(icons["info"], QString::fromLocal8Bit("信息"), this))
    , showSimulateAction(new QAction(icons["simu"], QString::fromLocal8Bit("仿真"), this))
    , showCalibrateAction(new QAction(icons["cali"], QString::fromLocal8Bit("标定"), this))
    , showLogAction(new QAction(icons["log"], QString::fromLocal8Bit("日志"), this))
    , showSystemAction(new QAction(icons["sys"], QString::fromLocal8Bit("系统"), this))

    // 主界面
    , main3DviewAction(new QAction(icons["d3"], QString::fromLocal8Bit("3D视角"), this))
    , mainComplateAction(new QAction(icons["compl"], QString::fromLocal8Bit("完成"), this))
    , mainCancelAction(new QAction(icons["cancel"], QString::fromLocal8Bit("取消"), this))
    , mainPLCAction(new QAction(icons["plc"], QString::fromLocal8Bit("PLC控制"), this))

    // 信息
    , taskAccepteAction(new QAction(icons["accept"], QString::fromLocal8Bit("接受"), this))
    , taskDeleteAction(new QAction(icons["cancel"], QString::fromLocal8Bit("取消"), this))
    , taskQueryAction(new QAction(icons["query"], QString::fromLocal8Bit("查询"), this))
    , taskPrintAction(new QAction(icons["print"], QString::fromLocal8Bit("打印"), this))
    , taskDetailAction(new QAction(icons["detail"], QString::fromLocal8Bit("详细"), this))

    // 仿真
    , simuForwardAction(new QAction(icons["right"], QString::fromLocal8Bit("下页"), this))
    , simuBackwardAction(new QAction(icons["left"], QString::fromLocal8Bit("上页"), this))
    , simulateStartAction(new QAction(icons["start"], QString::fromLocal8Bit("开始"), this))
    , simulateStopAction(new QAction(icons["stop"], QString::fromLocal8Bit("停止"), this))
    , simulateMapAction(new QAction(icons["open"], QString::fromLocal8Bit("打开"), this))
    , simulateModeAction(new QAction(icons["config"], QString::fromLocal8Bit("设置"), this))
    , simulateMLAction(new QAction(icons["ai"], QString::fromLocal8Bit("建模"), this))

    // 标定
    , calibrateStartAction(new QAction(icons["cali"], QString::fromLocal8Bit("开始"), this))
    , calibrateStopAction(new QAction(icons["stop"], QString::fromLocal8Bit("停止"), this))
    , calibrateSettingAction(new QAction(icons["config"], QString::fromLocal8Bit("设置"), this))
    , calibrateOpenAction(new QAction(icons["open"], QString::fromLocal8Bit("打开"), this))
    , calibrateSaveAction(new QAction(icons["save"], QString::fromLocal8Bit("保存"), this))
    , calibrateAddImageAction(new QAction(icons["add"], QString::fromLocal8Bit("添加"), this))
    , calibrateRemoveImageAction(new QAction(icons["cancel"], QString::fromLocal8Bit("删除"), this))
    , calibrateDetectAction(new QAction(icons["detect"], QString::fromLocal8Bit("模板"), this))
    , calibrateVideoAction(new QAction(icons["video"], QString::fromLocal8Bit("视频"), this))

    // 系统日志
    , logAllDataAction(new QAction(icons["all"], QString::fromLocal8Bit("全部日志"), this))
    , logRunningAction(new QAction(icons["run"], QString::fromLocal8Bit("运行日志"), this))
    , logSystemAction(new QAction(icons["alarm"], QString::fromLocal8Bit("系统报警"), this))

    // 系统维护
    , systemConfigAction(new QAction(icons["config"], QString::fromLocal8Bit("系统配置"), this))
    , systemParamAction(new QAction(icons["param"], QString::fromLocal8Bit("系统参数"), this))
    , systemAddUserAction(new QAction(icons["user"], QString::fromLocal8Bit("用户管理"), this))
    , systemMaintainAction(new QAction(icons["hardware"], QString::fromLocal8Bit("硬件维护"), this))

    , aboutAction(new QAction(icons["about"], QString::fromLocal8Bit("关于..."), this))
    , exitAction(new QAction(icons["exit"], QString::fromLocal8Bit("关闭"), this))
{
    viewActions->addAction(showMainPageAction);
    viewActions->addAction(showTaskAction);
    viewActions->addAction(showCalibrateAction);
    viewActions->addAction(showSimulateAction);
    viewActions->addAction(showLogAction);
    viewActions->addAction(showSystemAction);
    showMainPageAction->setCheckable(true);
    showTaskAction->setCheckable(true);
    showCalibrateAction->setCheckable(true);
    showSimulateAction->setCheckable(true);
    showLogAction->setCheckable(true);
    showSystemAction->setCheckable(true);
    viewActions->setExclusive(true);
    showMainPageAction->setChecked(true);

    mainActions->addAction(main3DviewAction);
    mainActions->addAction(mainComplateAction);
    mainActions->addAction(mainCancelAction);
    mainActions->addAction(mainPLCAction);
    mainActions->setExclusive(true);

    taskActions->addAction(taskAccepteAction);
    taskActions->addAction(taskDeleteAction);
    taskActions->addAction(taskQueryAction);
    taskActions->addAction(taskDetailAction);
    taskActions->addAction(taskPrintAction);
    taskActions->setExclusive(true);

    calibrateActions->addAction(calibrateStartAction);
    calibrateActions->addAction(calibrateStopAction);
    calibrateActions->addAction(calibrateSettingAction);
    calibrateActions->addAction(calibrateOpenAction);
    calibrateActions->addAction(calibrateSaveAction);
    calibrateActions->addAction(calibrateAddImageAction);
    calibrateActions->addAction(calibrateRemoveImageAction);
    calibrateActions->addAction(calibrateDetectAction);
    calibrateActions->addAction(calibrateVideoAction);
    calibrateStartAction->setChecked(false);
    calibrateActions->setExclusive(true);

    mainActions->addAction(simuForwardAction);
    mainActions->addAction(simuBackwardAction);
    simulateActions->addAction(simulateStartAction);
    simulateActions->addAction(simulateStopAction);
    simulateActions->addAction(simulateMapAction);
    simulateActions->addAction(simulateModeAction);
    simulateActions->addAction(simulateMLAction);
    simulateStartAction->setChecked(false);
    simulateActions->setExclusive(true);

    logActions->addAction(logAllDataAction);
    logActions->addAction(logRunningAction);
    logActions->addAction(logSystemAction);

    systemActions->addAction(systemConfigAction);
    systemActions->addAction(systemParamAction);
    systemActions->addAction(systemAddUserAction);
    systemActions->addAction(systemMaintainAction);

    helpActions->addAction(aboutAction);
}

VcActions::~VcActions()
{

}

QAction *VcActions::createAction(const QString &iconName, const QString &text)
{
    return new QAction(icons[iconName], text, this);
}

QAction *VcActions::createAction(const QString &iconName, const QString &text, const QKeySequence &shortcut)
{
    QAction *action = createAction(iconName, text);
    action->setShortcut(shortcut);
    return action;
}
