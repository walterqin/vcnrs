#ifndef CONNECTIONPAGE_H
#define CONNECTIONPAGE_H

#include <QWidget>

namespace Ui {
class ConnectionPage;
}

class DalsaG3GC11;
class Workbench;
class QListWidgetItem;

class ConnectionPage : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionPage(DalsaG3GC11 *camera, QWidget *parent = 0);
    ~ConnectionPage();

    Workbench *workbench() const;

signals:

public slots:
    void on_reset_clicked();
    void on_reconnect_clicked();
    void on_port_currentIndexChanged(const QString &port);
    void on_send_clicked();
    void on_log_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::ConnectionPage *ui;
    DalsaG3GC11 *m_instr;
};

#endif // CONNECTIONPAGE_H
