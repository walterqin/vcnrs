#include "debugpage.h"
#include "ui_debugpage.h"

DebugPage::DebugPage(DalsaG3GC11 *camera, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DebugPage)
{
    ui->setupUi(this);
}

DebugPage::~DebugPage()
{
    delete ui;
}

void DebugPage::addPage(QWidget *page)
{
    ui->tab->addTab(page, page->windowTitle());
}
