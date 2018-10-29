/**
* @file generalparamwidget.cpp
* @brief 通用参数类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2016-01-10
*/
#include "generalparamwidget.h"
#include "ui_generalparamwidget.h"
#include "profile.h"

GeneralParamWidget::GeneralParamWidget(Workbench *wb, QWidget *parent)
    : OptionWidget(wb, parent)
    , ui(new Ui::GeneralParamWidget)
{
    this->setWindowTitle(QString::fromLocal8Bit("通用参数设置"));
    this->setWindowModality(Qt::WindowModal);
    ui->setupUi(this);
    initializePage();
    connect(ui->saveBtn, SIGNAL(clicked()), this, SLOT(save()));
}

GeneralParamWidget::~GeneralParamWidget()
{
    delete ui;
}

void GeneralParamWidget::initializePage()
{

}

void GeneralParamWidget::save()
{

}
