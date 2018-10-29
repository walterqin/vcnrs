#include "connectionpage.h"
#include "ui_connectionpage.h"
#include "profile.h"
#include "vcapp.h"
#include "workbench.h"
#include "dalsag3gc11.h"
//#include "global.h"
#include "instrument.h"

ConnectionPage::ConnectionPage(DalsaG3GC11 *camera, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConnectionPage)
    , m_instr(camera)
{
    ui->setupUi(this);

}

ConnectionPage::~ConnectionPage()
{
    delete ui;
}

Workbench *ConnectionPage::workbench() const
{
    return m_instr->workbench();
}

void ConnectionPage::on_reconnect_clicked()
{
    WaitCursor wait;
    Instrument *ins = m_instr;
    ins->disconnectFromHost();
    ins->connectToHost();
    ins->waitForConnected();
    if (ins->isConnected())
        workbench()->information(tr("连接成功"));
    else
        workbench()->information(tr("连接失败"));
}

void ConnectionPage::on_port_currentIndexChanged(const QString &port)
{
    profile.setValue("instrument/port", port);
}

void ConnectionPage::on_reset_clicked()
{
    WaitCursor wait;
    Instrument *ins = m_instr;
    if (!ins)
        return;
    ins->reset();
}

void ConnectionPage::on_send_clicked()
{

}

void ConnectionPage::on_log_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item)
        return;

    ui->cmd->setText(item->text().mid(item->text().indexOf(": ")));
    ui->cmd->setFocus();
    ui->cmd->selectAll();
}
