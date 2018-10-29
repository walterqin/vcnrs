/**
* @file mainwindow.h
* @brief 应用程序窗体主界面类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMessageBox;
class MessageWidget;
class QToolBar;
class QCloseEvent;
class QToolButton;
class Workbench;

namespace Ui {
class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(Workbench *workbench, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();

    Workbench *workbench() const { return m_workbench; }

    QToolBar *toolBar() const;
    MessageWidget *messageWidget() const { return m_messageWidget; }

signals:
    void closed(QWidget *widget);

protected:
    virtual void closeEvent(QCloseEvent *e);

public slots:

private:
    Ui::MainWindowClass *ui;
    MessageWidget *m_messageWidget;
    Workbench *m_workbench;
};

#endif // MAINWINDOW_H
