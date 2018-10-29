/**
* @file logdataframe.cpp
* @brief 日志文件查询窗体类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#include <QtGui>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSplitter>
#include <QTextEdit>
#include <QMainWindow>

#include "logdataframe.h"
#include "workbench.h"
#include "profile.h"

LogDataFrame::LogDataFrame(Workbench *workbench, QWidget *parent)
    : MainWindowBase(parent)
    , m_workbench(workbench)
{
    QIcon logWarmIcon(style()->standardPixmap(QStyle::SP_MessageBoxWarning));
    QIcon logCritialIcon(style()->standardPixmap(QStyle::SP_MessageBoxCritical));
    QIcon logInfoIcon(style()->standardPixmap(QStyle::SP_MessageBoxInformation));

    QStringList logTypeLbl;
    logTypeLbl << QString::fromLocal8Bit("系统日志");

    m_logTypeWidget = new QTreeWidget;
    m_logTypeWidget->setHeaderLabels(logTypeLbl);
    addLogType(logCritialIcon, QString::fromLocal8Bit("紧急"));
    addLogType(logWarmIcon, QString::fromLocal8Bit("警告"));
    addLogType(logInfoIcon, QString::fromLocal8Bit("提示"));

    QStringList messageLabels;
    messageLabels << QString::fromLocal8Bit(" 概    要 ")
                  << QString::fromLocal8Bit(" 生  成  者 ")
                  << QString::fromLocal8Bit(" 日    期 ");

    m_logBriefWidget = new QTreeWidget;
    m_logBriefWidget->setHeaderLabels(messageLabels);
    m_logBriefWidget->resizeColumnToContents(0);
    m_logBriefWidget->resizeColumnToContents(1);

    m_logDetail = new QTextEdit;
    m_logDetail->setReadOnly(true);

    m_logDataSplitter = new QSplitter(Qt::Vertical);
    m_logDataSplitter->addWidget(m_logBriefWidget);
    m_logDataSplitter->addWidget(m_logDetail);
    m_logDataSplitter->setStretchFactor(1, 1);

    m_mainSplitter = new QSplitter(Qt::Horizontal);
    m_mainSplitter->addWidget(m_logTypeWidget);
    m_mainSplitter->addWidget(m_logDataSplitter);
    m_mainSplitter->setStretchFactor(1, 1);

    setCentralWidget(m_mainSplitter);
}

LogDataFrame::~LogDataFrame()
{

}

void LogDataFrame::addLogType(const QIcon &icon, const QString &name)
{
    QTreeWidgetItem *root;
    if (m_logTypeWidget->topLevelItemCount() == 0)
    {
        root = new QTreeWidgetItem(m_logTypeWidget);
        root->setText(0, QString::fromLocal8Bit("日志类型"));
        m_logTypeWidget->setItemExpanded(root, true);
    }
    else
    {
        root = m_logTypeWidget->topLevelItem(0);
    }

    QTreeWidgetItem *newItem = new QTreeWidgetItem(root);
    newItem->setText(0, name);
    newItem->setIcon(0, icon);

    if (!m_logTypeWidget->currentItem())
        m_logTypeWidget->setCurrentItem(newItem);
}

void LogDataFrame::addMessage(const QString &subject, const QString &from,
                const QString &date)
{

}
