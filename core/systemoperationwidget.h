#ifndef SYSTEMOPERATIONWIDGET_H
#define SYSTEMOPERATIONWIDGET_H

#include "optionwidget.h"

class Workbench;

namespace Ui {
    class SystemOperationWidget;
}

class SystemOperationWidget : public OptionWidget
{
public:
    SystemOperationWidget(Workbench *wb, QWidget *parent = 0);
    ~SystemOperationWidget();

    void initializePage();

public slots:
    void save();

private:
    Ui::SystemOperationWidget *ui;
};

#endif // SYSTEMOPERATIONWIDGET_H
