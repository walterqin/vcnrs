#include <QDebug>
#include <QToolBar>
#include <QAction>

#include "mainpageframe.h"
#include "vcactions.h"
#include "workbench.h"
#include "mainpagewidget.h"

MainPageFrame::MainPageFrame(Workbench *wb, QWidget *parent)
    : MainWindowBase(parent)
    , m_workbench(wb)
{
    m_toolbar = createFixedToolbar();

    m_actions = this->workbench()->actionManager();
    m_toolbar->addAction(m_actions->main3DviewAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(m_actions->mainComplateAction);
    m_toolbar->addAction(m_actions->mainCancelAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(m_actions->mainPLCAction);

    m_mainWidget = new MainPageWidget(wb, parent);
    setCentralWidget(m_mainWidget);

    connect(m_actions->main3DviewAction, SIGNAL(triggered()), this, SLOT(onShow3D()));
    connect(m_actions->mainComplateAction, SIGNAL(triggered()), this, SLOT(onComplete()));
    connect(m_actions->mainCancelAction, SIGNAL(triggered()), this, SLOT(onCancel()));
    connect(m_actions->mainPLCAction, SIGNAL(triggered()), this, SLOT(onPlcCommunication()));
}

MainPageFrame::~MainPageFrame()
{

}

void MainPageFrame::onShow3D()
{

}

void MainPageFrame::onPlcCommunication()
{

}

void MainPageFrame::onComplete()
{

}

void MainPageFrame::onCancel()
{

}
