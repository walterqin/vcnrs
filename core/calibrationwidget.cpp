#include "calibrationwidget.h"
#include "ui_calibrationwidget.h"
#include "workbench.h"
#include "profile.h"

CalibrationWidget::CalibrationWidget(Workbench *wb, QWidget *parent)
    : QWidget(parent)
    , m_workbench(wb)
    , ui(new Ui::CalibrationWidget)
{
    ui->setupUi(this);
}

CalibrationWidget::~CalibrationWidget()
{

}

