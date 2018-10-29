/**
* @file generalparamwidget.h
* @brief 通用参数类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2016-01-10
*/
#ifndef GENERALPARAMWIDGET_H
#define GENERALPARAMWIDGET_H

#include <QWidget>
#include "optionwidget.h"

class Workbench;
namespace Ui {
class GeneralParamWidget;
}
class GeneralParamWidget : public OptionWidget
{
    Q_OBJECT

public:
    GeneralParamWidget(Workbench *wb, QWidget *parent = 0);
    ~GeneralParamWidget();

    void initializePage();

private slots:
    void save();

private:
    Ui::GeneralParamWidget *ui;
};

#endif // GENERALPARAMWIDGET_H
