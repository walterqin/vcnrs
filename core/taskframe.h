#ifndef TASKFRAME_H
#define TASKFRAME_H

#include <QObject>
#include "mainwindowbase.h"

class Workbench;
class TaskWidget;

class TaskFrame : public MainWindowBase
{
    Q_OBJECT

public:
    TaskFrame(Workbench *workbench, QWidget *parent = 0);
    ~TaskFrame();

    Workbench *workbench() const { return m_workbench; }

private:
    Workbench *m_workbench;
    ToolBar *m_toolbar;
};

#endif // TASKFRAME_H
