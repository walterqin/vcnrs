#ifndef CALIBRATIONFRAME_H
#define CALIBRATIONFRAME_H

#include "mainwindowbase.h"


class Workbench;
class CalibrationDoc;
class AnalysisTableWidget;
class CalibrationDocModel;
class CalibrationWidget;

/**

定标主界面

*/

class CalibrationFrame : public MainWindowBase
{
	Q_OBJECT

public:
    explicit CalibrationFrame(Workbench *workbench, QWidget *parent = 0);
    ~CalibrationFrame();

	Workbench *workbench() const { return m_workbench; }

private slots:
    void onOpenFile();
    void onOpenVideo();
    void onSaveResult();
    void onStart();
    void onStop();
    void onDetect();
    void onAddImage();
    void onRemoveImage();
    void onSetting();

private:
	Workbench *m_workbench;
//	CalibrationDocModel *m_model;
//	AnalysisTableWidget *m_view;
	ToolBar *m_toolbar;
    CalibrationWidget *m_widget;
};

#endif // CALIBRATIONFRAME_H
