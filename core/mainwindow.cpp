/**
* @file mainwindow.h
* @brief 应用程序窗体主界面类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/

#include <QAction>
#include <QCloseEvent>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolButton>
#include <QStyle>

#include "mainwindow.h"
#include "vcapp.h"
#include "workbench.h"
#include "messagewidget.h"
#include "ui_mainwindow.h"

/**
* @brief 构造函数
* @param [I] 平台指针
* @param [I] 父指针
* @param [I] 窗口标志
*/
MainWindow::MainWindow(Workbench *wb, QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
    , ui(new Ui::MainWindowClass)
    , m_workbench(wb)
{
    ui->setupUi(this);
    m_messageWidget = MessageWidget::instance();
    m_messageWidget->setParent(this, Qt::Tool);

    QStatusBar *statusBar = this->statusBar();

	{
		QToolButton *button = new QToolButton(statusBar);
		button->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView));
		button->setAutoRaise(true);
		connect(button, SIGNAL(clicked()), workbench(), SLOT(showDebugger()));
		statusBar->addPermanentWidget(button);
	}

    // 显示消息按钮
    {
        QToolButton *button = new QToolButton(statusBar);
        button->addAction(m_messageWidget->toggleAction());
        button->setDefaultAction(m_messageWidget->toggleAction());
        button->setAutoRaise(true);
        statusBar->addPermanentWidget(button);
    }
}

/**
* @brief 析构函数
*/
MainWindow::~MainWindow()
{
    delete ui;
}

/**
* @brief 获取窗口工具条类
*/
QToolBar *MainWindow::toolBar() const
{
    return ui->toolBar;
}

/**
* @brief 关闭窗口事件处理
* @param [I] 关闭事件
*/
void MainWindow::closeEvent(QCloseEvent *e)
{
    if (workbench()->canClose())
        emit closed(this);
    else
        e->ignore();

}
