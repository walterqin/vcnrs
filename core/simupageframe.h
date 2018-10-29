#ifndef SIMUPAGEFRAME_H
#define SIMUPAGEFRAME_H

#include <QObject>
#include "mainwindowbase.h"

class Workbench;
class QToolBar;
class SimuWidget;
class VcActions;
class QDir;

class SimuPageFrame : public MainWindowBase
{
    Q_OBJECT

public:
    SimuPageFrame(Workbench *wb, QWidget *parent = 0);
    ~SimuPageFrame();

    Workbench *workbench() const { return m_workbench; }

public slots:
    void onForward();
    void onBackward();
    void onSimuStart();
    void onSimuStop();
    void onSimuMode();
    void onSimuOpen();
    void onModeling();

private:
    Workbench *m_workbench;
    QToolBar *m_toolbar;
    SimuWidget *m_simuWidget;
    VcActions *m_actions;

    QString m_imgDir;
    QStringList m_imgList;
    QString m_curSelectedFile;
};

#endif // SIMUPAGEFRAME_H
