#ifndef NRMPARAMPAGE_H
#define NRMPARAMPAGE_H

#include "optionwidget.h"

namespace Ui {
class NrmParamPage;
}

class NRM;
class QCheckBox;

class NrmParamPage : public OptionWidget
{
    Q_OBJECT

public:
    NrmParamPage(NRM *nrm, QWidget *parent = 0);
    ~NrmParamPage();

    void initializePage();

private slots:
    void onReadParam();
    void onSaveParam();

private:
    Ui::NrmParamPage *ui;
    NRM *m_nrm;
};

#endif // NRMPARAMPAGE_H
