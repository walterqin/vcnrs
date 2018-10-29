#include <QToolBar>
#include <QAction>
#include <QToolButton>

#include "mainwindowbase.h"


//////////////////////////////////////////////////////////
/// \brief ToolBar Class
///
QToolButton *ToolBar::toolButtonForAction(QAction *action) const
{
    return qobject_cast<QToolButton*>(widgetForAction(action));
}

void ToolBar::setToolButtonStyle(QAction *action, Qt::ToolButtonStyle style)
{
    QToolButton *button = toolButtonForAction(action);
    if (button)
        button->setToolButtonStyle(style);
}

void ToolBar::setToolButtonAutoRepeat(QAction *action, bool on)
{
    QToolButton *button = toolButtonForAction(action);
    if (button)
        button->setAutoRepeat(on);
}


///////////////////////////////////////////////////////////////////
/// \brief MainWindowBase Class
///
MainWindowBase::MainWindowBase(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    if (parent)
        setWindowFlags(windowFlags() & ~Qt::Window);
}

MainWindowBase::~MainWindowBase()
{

}

ToolBar *MainWindowBase::createFixedToolbar()
{
    ToolBar *toolbar = new ToolBar(this);
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setIconSize(QSize(32, 32));
    toolbar->toggleViewAction()->setEnabled(false);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addToolBar(Qt::TopToolBarArea, toolbar);

    return toolbar;
}
