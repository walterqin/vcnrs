#ifndef SIMUWIDGET_H
#define SIMUWIDGET_H

#include <QObject>
#include <QWidget>

#include "opencv/cv.h"
#include "opencv/highgui.h"


namespace Ui {
    class SimuWidget;
}

class Workbench;
class QDir;
class QString;

class SimuWidget : public QWidget
{
    Q_OBJECT
public:
    SimuWidget(Workbench *wb, QWidget *parent = 0);
    ~SimuWidget();

    bool showImage(QString &filename);

signals:

private slots:

private:
    QImage ImageCV2Qimg(IplImage* img);
    void initInfoTable();

private:
    Workbench *m_workbench;
    Ui::SimuWidget *ui;

    QString m_imgPath;
};

#endif // SIMUWIDGET_H
