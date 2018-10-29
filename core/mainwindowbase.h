#ifndef MAINWINDOWBASE_H
#define MAINWINDOWBASE_H

#include <QMainWindow>
#include <QToolBar>

class QAction;
class QToolButton;

class ToolBar : public QToolBar
{
    Q_OBJECT

public:
    ToolBar(QWidget *parent = 0) : QToolBar(parent) {}

    QToolButton *toolButtonForAction(QAction *action) const;

    using QToolBar::setToolButtonStyle;
    void setToolButtonStyle(QAction *action, Qt::ToolButtonStyle style);

    void setToolButtonAutoRepeat(QAction *action, bool on = true);
};

class MainWindowBase : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindowBase(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindowBase();

protected:
    ToolBar *createFixedToolbar();
};

#endif // MAINWINDOWBASE_H
