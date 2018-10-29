#include <QHeaderView>
#include <QToolBar>
#include <QAction>

#include "calibrationframe.h"
//#include "calibrationdocmodel.h"
//#include "calibrationdoc.h"
#include "workbench.h"
#include "vcapp.h"
#include "profile.h"
#include "vcactions.h"
#include "calibrationwidget.h"



CalibrationFrame::CalibrationFrame(Workbench *wb, QWidget *parent)
	: MainWindowBase(parent)
	, m_workbench(wb)
{
//	CalibrationDoc *doc = m_workbench->calibrationDoc();
	m_toolbar = createFixedToolbar();

    VcActions *actions = m_workbench->actionManager();
    m_toolbar->addAction(actions->calibrateOpenAction);
    m_toolbar->addAction(actions->calibrateAddImageAction);
    m_toolbar->addAction(actions->calibrateRemoveImageAction);
    m_toolbar->addAction(actions->calibrateVideoAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(actions->calibrateDetectAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(actions->calibrateStartAction);
    m_toolbar->addAction(actions->calibrateStopAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(actions->calibrateSaveAction);
    m_toolbar->addAction(actions->calibrateSettingAction);

    m_widget = new CalibrationWidget(wb, this);
    setCentralWidget(m_widget);

//	m_model = new CalibrationDocModel(doc, this);
//	QStringList assayList = profile.analysis_display_assay()
//		.split(';', QString::SkipEmptyParts);
//	foreach(const QString &assay, assayList)
//		m_model->insertAssay(assay);

//	m_view = new AnalysisTableWidget(workbench(), this);
//	m_view->setModel(m_model);
//	//m_view->resizeColumnsToContents();
//	m_view->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    connect(actions->calibrateOpenAction, SIGNAL(triggered()), this, SLOT(onOpenFile()));
    connect(actions->calibrateAddImageAction, SIGNAL(triggered()), this, SLOT(onAddImage()));
    connect(actions->calibrateRemoveImageAction, SIGNAL(triggered()), this, SLOT(onRemoveImage()));
    connect(actions->calibrateVideoAction, SIGNAL(triggered()), this, SLOT(onOpenVideo()));
    connect(actions->calibrateDetectAction, SIGNAL(triggered()), this, SLOT(onDetect()));
    connect(actions->calibrateStartAction, SIGNAL(triggered()), this, SLOT(onStart()));
    connect(actions->calibrateStopAction, SIGNAL(triggered()), this, SLOT(onStop()));
    connect(actions->calibrateSaveAction, SIGNAL(triggered()), this, SLOT(onSaveResult()));
    connect(actions->calibrateSettingAction, SIGNAL(triggered()), this, SLOT(onSetting()));

}

CalibrationFrame::~CalibrationFrame()
{

}

void CalibrationFrame::onOpenFile()
{

}

void CalibrationFrame::onOpenVideo()
{

}

void CalibrationFrame::onSaveResult()
{

}

void CalibrationFrame::onStart()
{

}

void CalibrationFrame::onStop()
{

}

void CalibrationFrame::onDetect()
{

}

void CalibrationFrame::onAddImage()
{

}

void CalibrationFrame::onRemoveImage()
{

}

void CalibrationFrame::onSetting()
{

}
