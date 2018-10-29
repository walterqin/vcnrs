#ifndef INSTRUMENTPARAMWIDGET_H
#define INSTRUMENTPARAMWIDGET_H

#include "optionwidget.h"

class Workbench;

namespace Ui {
class InstrumentParamWidget;
}

class CORE_EXPORT InstrumentParamWidget : public OptionWidget
{
public:
    InstrumentParamWidget(Workbench *wb, QWidget *parent = 0);
    ~InstrumentParamWidget();

    void initializePage();

private:
    Ui::InstrumentParamWidget *ui;
};

#endif // INSTRUMENTPARAMWIDGET_H
