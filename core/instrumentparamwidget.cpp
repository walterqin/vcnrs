#include "instrumentparamwidget.h"
#include "ui_instrumentparamwidget.h"
#include "workbench.h"
#include "profile.h"

InstrumentParamWidget::InstrumentParamWidget(Workbench *wb, QWidget *parent)
    : OptionWidget(wb, parent)
    , ui(new Ui::InstrumentParamWidget)
{
    ui->setupUi(this);
    initializePage();
}

InstrumentParamWidget::~InstrumentParamWidget()
{
    delete ui;
}

void InstrumentParamWidget::initializePage()
{

}
