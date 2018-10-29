#ifndef CALIBRATIONWIDGET_H
#define CALIBRATIONWIDGET_H

#include <QWidget>

class Workbench;

namespace Ui {
class CalibrationWidget;
}

class CalibrationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalibrationWidget(Workbench *wb, QWidget *parent = 0);
    ~CalibrationWidget();

    Workbench *workbench() const { return m_workbench; }

signals:

public slots:

private:
    Workbench *m_workbench;
    Ui::CalibrationWidget *ui;
};

#endif // CALIBRATIONWIDGET_H
