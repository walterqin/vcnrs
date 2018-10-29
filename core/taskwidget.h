#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>

class Workbench;
class QDir;
class QString;

class TaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskWidget(Workbench *wb, QWidget *parent = 0);
    ~TaskWidget();

signals:

public slots:
};

#endif // TASKWIDGET_H
