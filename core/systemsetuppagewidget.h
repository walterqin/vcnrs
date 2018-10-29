#ifndef SYSTEMSETUPPAGEWIDGET_H
#define SYSTEMSETUPPAGEWIDGET_H

#include "optionwidget.h"


class Workbench;

namespace Ui{
    class SystemSetupPageWidget;
}

class SystemSetupPageWidget : public OptionWidget
{
    Q_OBJECT

public:
    SystemSetupPageWidget(Workbench *wb, QWidget *parent = 0);
    ~SystemSetupPageWidget();

    void initializePage();

signals:

private slots:
    void save();

private:
    Ui::SystemSetupPageWidget *ui;
};

#endif // SYSTEMSETUPPAGEWIDGET_H
