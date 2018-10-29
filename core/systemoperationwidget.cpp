#include "systemoperationwidget.h"
#include "ui_systemoperationwidget.h"
#include "workbench.h"
#include "profile.h"

SystemOperationWidget::SystemOperationWidget(Workbench *wb, QWidget *parent)
    : OptionWidget(wb, parent)
    , ui(new Ui::SystemOperationWidget)
{
    ui->setupUi(this);
    initializePage();
}

SystemOperationWidget::~SystemOperationWidget()
{
    delete ui;
}

void SystemOperationWidget::initializePage()
{

}

void SystemOperationWidget::save()
{

}
