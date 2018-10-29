/**
* @file networkconfigwidget.cpp
* @brief 网络传输设置类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2016-01-10
*/
#include "profile.h"
#include "networkconfigwidget.h"
#include "ui_networkconfigwidget.h"

NetworkConfigWidget::NetworkConfigWidget(Workbench *wb, QWidget *parent)
    : OptionWidget(wb, parent)
    , ui(new Ui::NetworkConfigWidget)
{
    ui->setupUi(this);
    initializePage();
    connect(ui->saveBtn, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->restoreBtn, SIGNAL(clicked()), this, SLOT(restore()));
}

NetworkConfigWidget::~NetworkConfigWidget()
{
    delete ui;
}

void NetworkConfigWidget::initializePage()
{

}

void NetworkConfigWidget::save()
{

}

void NetworkConfigWidget::restore()
{

}
