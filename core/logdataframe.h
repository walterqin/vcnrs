/**
* @file logdataframe.h
* @brief 日志文件查询窗体类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#ifndef LOGDATAFRAME_H
#define LOGDATAFRAME_H

#include "mainwindowbase.h"

class Workbench;
class QSplitter;
class QTextEdit;
class QTreeWidget;

class LogDataFrame : public MainWindowBase
{
    Q_OBJECT

public:
    explicit LogDataFrame(Workbench *workbench, QWidget *parent = 0);
    ~LogDataFrame();

public slots:

private:
    void addLogType(const QIcon &icon, const QString &name);
    void addMessage(const QString &subject, const QString &from,
                    const QString &date);

private:
    Workbench *m_workbench;

    QTreeWidget *m_logTypeWidget;
    QTreeWidget *m_logBriefWidget;
    QTextEdit *m_logDetail;

    QSplitter *m_mainSplitter;
    QSplitter *m_logDataSplitter;
};

#endif // LOGDATAFRAME_H
