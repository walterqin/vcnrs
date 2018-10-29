#ifndef LOGDATAWIDGET_H
#define LOGDATAWIDGET_H

#include <QWidget>

class QSplitter;
class QTextEdit;
class QTreeWidget;

class LogDataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogDataWidget(QWidget *parent = 0);
    ~LogDataWidget();

signals:

public slots:

private:
    void addLogType(const QIcon &icon, const QString &name);
    void addMessage(const QString &subject, const QString &from,
                    const QString &date);

private:
    QTreeWidget *m_logTypeWidget;
    QTreeWidget *m_logBriefWidget;
    QTextEdit *m_logDetail;

    QSplitter *m_mainSplitter;
    QSplitter *m_logDataSplitter;
};

#endif // LOGDATAWIDGET_H
