#ifndef MAINPAGEFRAME_H
#define MAINPAGEFRAME_H

#include "mainwindowbase.h"

class Workbench;
class QToolBar;
class MainPageWidget;
class VcActions;

class MainPageFrame : public MainWindowBase
{
    Q_OBJECT

public:
    MainPageFrame(Workbench *wb, QWidget *parent = 0);
    ~MainPageFrame();

    Workbench *workbench() const { return m_workbench; }

private slots:
    void onShow3D();
    void onPlcCommunication();
    void onComplete();
    void onCancel();

private:
    Workbench *m_workbench;
    QToolBar *m_toolbar;
    MainPageWidget *m_mainWidget;
    VcActions *m_actions;
};

#endif // MAINPAGEFRAME_H
