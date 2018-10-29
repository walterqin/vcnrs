#include "parampage.h"
#include "ui_parampage.h"
#include "dalsag3gc11.h"

ParamPage::ParamPage(DalsaG3GC11 *camera, QWidget *parent)
    : OptionWidget(camera->workbench(), parent),
      ui(new Ui::ParamPage)
{
    ui->setupUi(this);
}

ParamPage::~ParamPage()
{
    delete ui;
}

void ParamPage::initializePage()
{

}

void ParamPage::onReadParam()
{

}

void ParamPage::onSaveParam()
{

}

bool ParamPage::readParam()
{
	return true;
}

void ParamPage::setMessage(const QIcon &icon, const QString &text)
{

}
