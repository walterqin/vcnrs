#include <QToolBar>
#include <QAction>

#include "taskframe.h"
#include "vcactions.h"
#include "workbench.h"
#include "taskwidget.h"

TaskFrame::TaskFrame(Workbench *workbench, QWidget *parent)
    : MainWindowBase(parent)
    , m_workbench(workbench)
{
    m_toolbar = createFixedToolbar();

    VcActions *actions = m_workbench->actionManager();
    m_toolbar->addAction(actions->taskAccepteAction);
    m_toolbar->addAction(actions->taskDeleteAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(actions->taskQueryAction);
    m_toolbar->addAction(actions->taskDetailAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(actions->taskPrintAction);
}

TaskFrame::~TaskFrame()
{

}
