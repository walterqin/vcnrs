#include <QDebug>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QModelIndex>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QtEvents>

#include "mainpagewidget.h"
#include "ui_mainpagewidget.h"
#include "workbench.h"
#include "vcactions.h"

MainPageWidget::MainPageWidget(Workbench *wb, QWidget *parent)
    : QWidget(parent)
    , m_workbench(wb)
    , ui(new Ui::MainPageWidget)
{
    ui->setupUi(this);

    m_VideoType = "1_4";

    initVideo();
    initInfoTable();

    changeVideoLayout();
}

MainPageWidget::~MainPageWidget()
{
    delete ui;
}

void MainPageWidget::initVideo()
{
    m_videoMax = false;

    VideoLab.append(ui->labVideo1);
    VideoLab.append(ui->labVideo2);
    VideoLab.append(ui->labVideo3);
    VideoLab.append(ui->labVideo4);
    VideoLab.append(ui->labVideo5);
    VideoLab.append(ui->labVideo6);
    VideoLab.append(ui->labVideo7);
    VideoLab.append(ui->labVideo8);
    VideoLab.append(ui->labVideo9);
    VideoLab.append(ui->labVideo10);
    VideoLab.append(ui->labVideo11);
    VideoLab.append(ui->labVideo12);
    VideoLab.append(ui->labVideo13);
    VideoLab.append(ui->labVideo14);
    VideoLab.append(ui->labVideo15);
    VideoLab.append(ui->labVideo16);

    VideoLay.append(ui->lay1);
    VideoLay.append(ui->lay2);
    VideoLay.append(ui->lay3);
    VideoLay.append(ui->lay4);

    for (int i = 0; i < 16; i++) {
        VideoLab[i]->installEventFilter(this);
        VideoLab[i]->setProperty("labVideo", true);
        VideoLab[i]->setText(QString::fromLocal8Bit("相机%1").arg(i + 1));
    }
}

void MainPageWidget::initInfoTable()
{
//    QStringList header;
//    header << tr("序号") << tr("箱号") << tr("箱主") << tr("箱型");

//    ui->infoTable->setHorizontalHeaderLabels(header);
    ui->infoTable->horizontalHeader()->setVisible(true);
    ui->infoTable->verticalHeader()->setVisible(false);
    ui->infoTable->setColumnCount(4);
    ui->infoTable->setRowCount(2);
    ui->infoTable->setColumnWidth(0, 50);
    ui->infoTable->setColumnWidth(1, 200);
    ui->infoTable->setColumnWidth(2, 100);
    ui->infoTable->setColumnWidth(3, 50);
}

void MainPageWidget::loadVideo()
{
    changeVideoLayout();
}

void MainPageWidget::removelayout()
{
    for (int i = 0; i < 4; i++) {
        VideoLay[0]->removeWidget(VideoLab[i]);
        VideoLab[i]->setVisible(false);
    }

    for (int i = 4; i < 8; i++) {
        VideoLay[1]->removeWidget(VideoLab[i]);
        VideoLab[i]->setVisible(false);
    }

    for (int i = 8; i < 12; i++) {
        VideoLay[2]->removeWidget(VideoLab[i]);
        VideoLab[i]->setVisible(false);
    }

    for (int i = 12; i < 16; i++) {
        VideoLay[3]->removeWidget(VideoLab[i]);
        VideoLab[i]->setVisible(false);
    }
}

void MainPageWidget::changeVideo1(int index)
{
    for (int i = (index + 0); i < (index + 1) ; i++) {
        VideoLay[0]->addWidget(VideoLab[i]);
        VideoLab[i]->setVisible(true);
    }
}

void MainPageWidget::changeVideo2(int index)
{
    if (index != 0)
        index = 0;

    for (int i = (index + 0); i < (index + 2); i++) {
        VideoLay[0]->addWidget(VideoLab[i]);
        VideoLab[i]->setVisible(true);
    }
}

void MainPageWidget::changeVideo4(int index)
{
    for (int i = (index + 0); i < (index + 2); i++) {
        VideoLay[0]->addWidget(VideoLab[i]);
        VideoLab[i]->setVisible(true);
    }

    for (int i = (index + 2); i < (index + 4); i++) {
        VideoLay[1]->addWidget(VideoLab[i]);
        VideoLab[i]->setVisible(true);
    }
}

void MainPageWidget::changeVideo9(int index)
{

}

void MainPageWidget::changeVideo16(int index)
{

}

void MainPageWidget::changeVideoLayout()
{
    if (m_VideoType == "1") {
        removelayout();
        changeVideo1(0);
    } else if (m_VideoType == "2") {
        removelayout();
        changeVideo2(0);
    } else if (m_VideoType == "1_4") {
        removelayout();
        changeVideo4(0);
    } else if (m_VideoType == "5_8") {
        removelayout();
        changeVideo4(4);
    } else if (m_VideoType == "9_12") {
        removelayout();
        changeVideo4(8);
    } else if (m_VideoType == "13_16") {
        removelayout();
        changeVideo4(12);
    } else if (m_VideoType == "1_9") {
        removelayout();
        changeVideo9(0);
    } else if (m_VideoType == "8_16") {
        removelayout();
        changeVideo9(7);
    } else if (m_VideoType == "16") {
        removelayout();
        changeVideo16(0);
    }
}

void MainPageWidget::keyPressEvent(QKeyEvent *event)
{
    //空格键进入全屏,esc键退出全屏
    switch(event->key()) {
    case Qt::Key_F1:
        on_screen_full();
        break;
    case Qt::Key_Escape:
        on_screen_normal();
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

bool MainPageWidget::eventFilter(QObject *obj, QEvent *event)
{
    QMouseEvent *MouseEvent = static_cast<QMouseEvent *>(event);
    if ((event->type() == QEvent::MouseButtonDblClick) &&
            (MouseEvent->buttons() == Qt::LeftButton)) {
        QLabel *labDouble = qobject_cast<QLabel *>(obj);
        if (!m_videoMax) {
            removelayout();
            m_videoMax = true;
            VideoLay[0]->addWidget(labDouble);
            labDouble->setVisible(true);
        } else {
            m_videoMax = false;
            changeVideoLayout();
        }

        labDouble->setFocus();
        return true;
    }

    return QObject::eventFilter(obj, event);
}

void MainPageWidget::on_screen_full()
{
//    this->setGeometry(qApp->desktop()->geometry());
    this->layout()->setContentsMargins(0, 0, 0, 0);
    ui->infoTable->setVisible(false);
}

void MainPageWidget::on_screen_normal()
{
//    this->setGeometry(qApp->desktop()->availableGeometry());
    this->layout()->setContentsMargins(1, 1, 1, 1);
    ui->infoTable->setVisible(true);
}

void MainPageWidget::on_show_video_1()
{
    qDebug() << "video1";
}

void MainPageWidget::on_show_video_4()
{
    qDebug() << "video4";
}

void MainPageWidget::on_show_video_9()
{

}

void MainPageWidget::on_show_video_16()
{

}
