/**
* @file networkconfigwidget.h
* @brief 网络传输设置类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2016-01-10
*/
#ifndef NETWORKCONFIGWIDGET_H
#define NETWORKCONFIGWIDGET_H

#include "optionwidget.h"

class Workbench;

namespace Ui {
    class NetworkConfigWidget;
}
class NetworkConfigWidget : public OptionWidget
{
    Q_OBJECT

public:
    NetworkConfigWidget(Workbench *wb, QWidget *parent = 0);
    ~NetworkConfigWidget();

    void initializePage();

private slots:
    void save();
    void restore();

private:
    Ui::NetworkConfigWidget *ui;
};

#endif // NETWORKCONFIGWIDGET_H
