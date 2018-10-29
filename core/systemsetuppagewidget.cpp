#include "profile.h"
#include "systemsetuppagewidget.h"
#include "ui_systemsetuppagewidget.h"
#include "workbench.h"

SystemSetupPageWidget::SystemSetupPageWidget(Workbench *wb, QWidget *parent)
    : OptionWidget(wb, parent)
    , ui(new Ui::SystemSetupPageWidget)
{
    ui->setupUi(this);
    initializePage();
}


SystemSetupPageWidget::~SystemSetupPageWidget()
{
    delete ui;
}

void SystemSetupPageWidget::initializePage()
{

}

void SystemSetupPageWidget::save()
{

}
