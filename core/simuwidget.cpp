#include <QtCore>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <QPixmapCache>
#include <QWidget>

#include "simuwidget.h"
#include "ui_simuwidget.h"
#include "workbench.h"

SimuWidget::SimuWidget(Workbench *wb, QWidget *parent)
    : QWidget(parent)
    , m_workbench(wb)
    , ui(new Ui::SimuWidget)
    , m_imgPath(QString())
{
    ui->setupUi(this);

    initInfoTable();
}

SimuWidget::~SimuWidget()
{
    delete ui;
	QPixmapCache::clear();
}

void SimuWidget::initInfoTable()
{
    ui->infoTable->verticalHeader()->setVisible(false);
    ui->infoTable->setColumnWidth(0, 50);
    ui->infoTable->setColumnWidth(1, 200);
    ui->infoTable->setColumnWidth(2, 100);
    ui->infoTable->setColumnWidth(3, 50);
    ui->infoTable->setRowCount(2);
}

bool SimuWidget::showImage(QString &filename)
{
    if (filename.isEmpty())
        return false;

    m_imgPath = filename;

    IplImage *img = cvLoadImage(m_imgPath.toStdString().data());
    QImage qimg = ImageCV2Qimg(img);
    QPixmap pixmap;

    if (!QPixmapCache::find(m_imgPath, pixmap))
    {
        pixmap = QPixmap::fromImage(qimg);
        QPixmapCache::insert(m_imgPath, pixmap);
    }

    pixmap.scaled(ui->labVideo1->size(), Qt::KeepAspectRatio);
    ui->labVideo1->setScaledContents(true);
    ui->labVideo1->setPixmap(pixmap);
    ui->labVideo1->show();
}

QImage SimuWidget::ImageCV2Qimg(IplImage* img)
{
    assert(img != NULL);

    int h = img->height;
    int w = img->width;
    int channel = img->nChannels;
    QImage qimg = QImage(w, h, QImage::Format_RGB32);
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            int r, g, b;
            if(channel == 3)
            {
                b = (int)CV_IMAGE_ELEM(img, uchar, i, j*3+0);
                g = (int)CV_IMAGE_ELEM(img, uchar, i, j*3+1);
                r = (int)CV_IMAGE_ELEM(img, uchar, i, j*3+2);
            }
            else if(channel == 1)
            {
                b = (int)CV_IMAGE_ELEM(img, uchar, i, j);
                g = b;
                r = b;
            }
            qimg.setPixel(j, i, qRgb(r, g, b));
        }
    }
    return qimg;
}
