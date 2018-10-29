#include "nrmparampage.h"
#include "ui_nrmparampage.h"
#include "nrm.h"

NrmParamPage::NrmParamPage(NRM *nrm, QWidget *parent)
    : OptionWidget(nrm->workbench(), parent)
    , m_nrm(nrm)
    , ui(new Ui::NrmParamPage)
{
    ui->setupUi(this);
}

NrmParamPage::~NrmParamPage()
{
    delete ui;
}

void NrmParamPage::initializePage()
{

}

void NrmParamPage::onReadParam()
{

}

void NrmParamPage::onSaveParam()
{

}
