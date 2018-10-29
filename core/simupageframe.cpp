#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QMessageBox>
#include <QRegExp>

#include "opencv/cv.h"
#include "opencv/highgui.h"

#include "global.h"
#include "simupageframe.h"
#include "simuwidget.h"
#include "workbench.h"
#include "vcactions.h"
#include "numberrecognition.h"
#include "numberzone.h"

SimuPageFrame::SimuPageFrame(Workbench *wb, QWidget *parent)
    : MainWindowBase(parent)
    , m_workbench(wb)
    , m_imgDir(QApplication::applicationDirPath())
{
    m_toolbar = createFixedToolbar();

    m_actions = this->workbench()->actionManager();
    m_toolbar->addAction(m_actions->simuBackwardAction);
    m_toolbar->addAction(m_actions->simuForwardAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(m_actions->simulateStartAction);
    m_toolbar->addAction(m_actions->simulateStopAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(m_actions->simulateMapAction);
    m_toolbar->addAction(m_actions->simulateModeAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(m_actions->simulateMLAction);

    m_simuWidget = new SimuWidget(wb, parent);
    setCentralWidget(m_simuWidget);

    connect(m_actions->simuForwardAction, SIGNAL(triggered()), this, SLOT(onForward()));
    connect(m_actions->simuBackwardAction, SIGNAL(triggered()), this, SLOT(onBackward()));
    connect(m_actions->simulateStartAction, SIGNAL(triggered()), this, SLOT(onSimuStart()));
    connect(m_actions->simulateStopAction, SIGNAL(triggered()), this, SLOT(onSimuStop()));
    connect(m_actions->simulateMapAction, SIGNAL(triggered()), this, SLOT(onSimuOpen()));
    connect(m_actions->simulateModeAction, SIGNAL(triggered()), this, SLOT(onSimuMode()));
    connect(m_actions->simulateMLAction, SIGNAL(triggered()), this, SLOT(onModeling()));
}

SimuPageFrame::~SimuPageFrame()
{

}

void SimuPageFrame::onForward()
{
    if (m_imgList.count() > 0)
    {
        int pos = m_imgList.indexOf(m_curSelectedFile) + 1;
        if (pos >= m_imgList.count())
        {
            pos -= 1;
            information(QString::fromLocal8Bit("已经是最后一张"));
        }
        m_curSelectedFile = m_imgList.at(pos);
        m_simuWidget->showImage(m_curSelectedFile);
    }
}

void SimuPageFrame::onBackward()
{
    if (m_imgList.count() > 0)
    {
        int pos = m_imgList.indexOf(m_curSelectedFile) - 1;
        if (pos < 0)
        {
            pos = 0;
            information(QString::fromLocal8Bit("已经是第一张"));
        }
        m_curSelectedFile = m_imgList.at(pos);
        m_simuWidget->showImage(m_curSelectedFile);
    }
}

void SimuPageFrame::onSimuStart()
{
    if (m_curSelectedFile.isEmpty())
        return;

    QVector<NumberZone> out;
    IplImage *img = cvLoadImage(m_curSelectedFile.toStdString().data());
    Mat mat = cvarrToMat(img);
    NumberRecognition *nr = m_workbench->algorithm();
    nr->numberRecognize(mat, out);
}

void SimuPageFrame::onSimuStop()
{

}

void SimuPageFrame::onSimuMode()
{

}

void SimuPageFrame::onSimuOpen()
{
    QFileDialog img(0, QString::fromLocal8Bit("打开图像文件"),
                    m_imgDir,
                    "Image File[*.bmp *.png *.jpg *.jpeg]");

    if (img.exec() == QDialog::Accepted)
    {
        m_imgDir = img.directory().absolutePath();

        // 获取当前路径下所有得图片文件
        QDir dir = img.directory();
        QStringList filters;
        QStringList fnList;
        filters << "*.bmp" << "*.png" << "*.jpg" << "*.jpeg";
        fnList = dir.entryList(filters, QDir::Files|QDir::Readable, QDir::Name);
        foreach(QString fn, fnList)
            m_imgList << dir.absolutePath() + "/" + fn;

        // 显示选择的图片
        QStringList fileNames = img.selectedFiles();
        if (fileNames.size() > 0)
        {
            m_curSelectedFile = fileNames.first();
            m_simuWidget->showImage(m_curSelectedFile);
        }
    }
}

void SimuPageFrame::onModeling()
{

}
