#include "trainmodelpage.h"
#include "ui_trainmodelpage.h"
#include "nrm.h"
#include "profile.h"
#include "trainann.h"
#include "trainannch.h"
#include "trainsvm.h"

#include <QLineEdit>
#include <QFileDialog>
#include <QDir>

TrainModelPage::TrainModelPage(NRM *nrm, QWidget *parent)
    : OptionWidget(nrm->workbench(), parent)
    , m_nrm(nrm)
    , ui(new Ui::TrainModelPage)
{
    ui->setupUi(this);
    initializePage();

    connect(ui->trainSvmBtn, SIGNAL(clicked()), this, SLOT(onTrainSvm()));
    connect(ui->trainAnnBtn, SIGNAL(clicked()), this, SLOT(onTrainAnn()));
    connect(ui->trainAnnZhBtn, SIGNAL(clicked()), this, SLOT(onTrainAnnZh()));

    connect(ui->svmSelDataBtn, SIGNAL(clicked()), this, SLOT(onSvmSelDataTrainPath()));
    connect(ui->svmSelTestBtn, SIGNAL(clicked()), this, SLOT(onSvmSelTestTrainPath()));
    connect(ui->svmSelUndataBtn, SIGNAL(clicked()), this, SLOT(onSvmSelUndataTrainPath()));
    connect(ui->svmSelUnTestBtn, SIGNAL(clicked()), this, SLOT(onSvmSelUnTestTrainPath()));
    connect(ui->svmSelBtn, SIGNAL(clicked()), this, SLOT(onSvmSetModelFileName()));

    connect(ui->ann1SelDataBtn, SIGNAL(clicked()), this, SLOT(onAnn1SelDataTrainPath()));
    connect(ui->ann1SelTestBtn, SIGNAL(clicked()), this, SLOT(onAnn1SelTestTrainPath()));
    connect(ui->ann1SelUndataBtn, SIGNAL(clicked()), this, SLOT(onAnn1SelUndataTrainPath()));
    connect(ui->ann1SelUnTestBtn, SIGNAL(clicked()), this, SLOT(onAnn1SelUnTestTrainPath()));
    connect(ui->ann1SelBtn, SIGNAL(clicked()), this, SLOT(onAnn1SetModelFileName()));

    connect(ui->ann2SelDataBtn, SIGNAL(clicked()), this, SLOT(onAnn2SelDataTrainPath()));
    connect(ui->ann2SelTestBtn, SIGNAL(clicked()), this, SLOT(onAnn2SelTestTrainPath()));
    connect(ui->ann2SelUndataBtn, SIGNAL(clicked()), this, SLOT(onAnn2SelUndataTrainPath()));
    connect(ui->ann2SelUnTestBtn, SIGNAL(clicked()), this, SLOT(onAnn2SelUnTestTrainPath()));
    connect(ui->ann2SelBtn, SIGNAL(clicked()), this, SLOT(onAnn2SetModelFileName()));
}

TrainModelPage::~TrainModelPage()
{
    delete ui;
}

void TrainModelPage::initializePage()
{
    m_svmDataPath.clear();
    m_svmModelPath.clear();
    m_ann1DataPath.clear();
    m_ann1ModelPath.clear();
    m_ann2DataPath.clear();
    m_ann2ModelPath.clear();
}

void TrainModelPage::onTrainSvm()
{
    TrainSVM svm(m_svmDataPath, m_svmModelPath);

    svm.setTrainDataDir(m_svmDataPath);
    svm.setTrainTestDir(m_svmTestPath);
    svm.setTrainUndataDir(m_svmUndataPath);
    svm.setTrainUnTestDir(m_svmUnTestPath);
    svm.setModelDir(m_svmModelPath);

    svm.train();
}

void TrainModelPage::onTrainAnn()
{
    TrainANN ann(m_ann1DataPath, m_ann1ModelPath);

    ann.setTrainDataDir(m_ann1DataPath);
    ann.setTrainTestDir(m_ann1TestPath);
    ann.setTrainUndataDir(m_ann1UndataPath);
    ann.setTrainUnTestDir(m_ann1UnTestPath);
    ann.setModelDir(m_ann1ModelPath);

    ann.train();
}

void TrainModelPage::onTrainAnnZh()
{
    TrainAnnCh ann(m_ann2DataPath, m_ann2ModelPath);

    ann.setTrainDataDir(m_ann2DataPath);
    ann.setTrainTestDir(m_ann2TestPath);
    ann.setTrainUndataDir(m_ann2UndataPath);
    ann.setTrainUnTestDir(m_ann2UnTestPath);
    ann.setModelDir(m_ann2ModelPath);

    ann.train();
}

void TrainModelPage::onSvmSelDataTrainPath()
{
    m_svmDataPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if (fileDlg.exec() == QDialog::Accepted)
    {
        QDir dataDir = fileDlg.directory();
        m_svmDataPath = dataDir.absolutePath();
        ui->svmDataDir->setText(m_svmDataPath);
    }
    else
        m_svmDataPath = ui->svmDataDir->text();
}

void TrainModelPage::onSvmSelTestTrainPath()
{
    m_svmTestPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if (fileDlg.exec() == QDialog::Accepted)
    {
        QDir dataDir = fileDlg.directory();
        m_svmTestPath = dataDir.absolutePath();
        ui->svmTestDir->setText(m_svmTestPath);
    }
    else
        m_svmTestPath = ui->svmTestDir->text();
}

void TrainModelPage::onSvmSelUndataTrainPath()
{
    m_svmUndataPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if (fileDlg.exec() == QDialog::Accepted)
    {
        QDir dataDir = fileDlg.directory();
        m_svmUndataPath = dataDir.absolutePath();
        ui->svmUndataDir->setText(m_svmUndataPath);
    }
    else
        m_svmUndataPath = ui->svmUndataDir->text();
}

void TrainModelPage::onSvmSelUnTestTrainPath()
{
    m_svmUnTestPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if ( fileDlg.exec() == QDialog::Accepted )
    {
        QDir dataDir = fileDlg.directory();
        m_svmUnTestPath = dataDir.absolutePath();
        ui->svmUnTestDir->setText(m_svmUnTestPath);
    }
    else
        m_svmUnTestPath = ui->svmUnTestDir->text();
}

void TrainModelPage::onSvmSetModelFileName()
{
    m_svmModelPath.clear();
    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if ( fileDlg.exec() == QDialog::Accepted )
    {
        QDir dataDir = fileDlg.directory();
        m_svmModelPath = dataDir.absolutePath();
        ui->svmModelDir->setText(m_svmModelPath);
    }
    m_svmModelPath = ui->svmModelDir->text();
}

void TrainModelPage::onAnn1SelDataTrainPath()
{
    m_ann1DataPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if ( fileDlg.exec() == QDialog::Accepted )
    {
        QDir dataDir = fileDlg.directory();
        m_ann1DataPath = dataDir.absolutePath();
        ui->ann1DataDir->setText(m_ann1DataPath);
    }
    m_ann1DataPath = ui->ann1DataDir->text();
}

void TrainModelPage::onAnn1SelTestTrainPath()
{
    m_ann1TestPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if ( fileDlg.exec() == QDialog::Accepted )
    {
        QDir dataDir = fileDlg.directory();
        m_ann1TestPath = dataDir.absolutePath();
        ui->ann1TestDir->setText(m_ann1TestPath);
    }
    m_ann1TestPath = ui->ann1TestDir->text();
}

void TrainModelPage::onAnn1SelUndataTrainPath()
{
    m_ann1UndataPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if ( fileDlg.exec() == QDialog::Accepted )
    {
        QDir dataDir = fileDlg.directory();
        m_ann1UndataPath = dataDir.absolutePath();
        ui->ann1UndataDir->setText(m_ann1UndataPath);
    }
    m_ann1UndataPath = ui->ann1UndataDir->text();
}

void TrainModelPage::onAnn1SelUnTestTrainPath()
{
    m_ann1UnTestPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if ( fileDlg.exec() == QDialog::Accepted )
    {
        QDir dataDir = fileDlg.directory();
        m_ann1UnTestPath = dataDir.absolutePath();
        ui->ann1UnTestDir->setText(m_ann1UnTestPath);
    }
    m_ann1UnTestPath = ui->ann1UnTestDir->text();
}

void TrainModelPage::onAnn1SetModelFileName()
{
    m_ann1ModelPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if ( fileDlg.exec() == QDialog::Accepted )
    {
        QDir dataDir = fileDlg.directory();
        m_ann1ModelPath = dataDir.absolutePath();
        ui->ann1ModelDir->setText(m_ann1ModelPath);
    }
    m_ann1ModelPath = ui->ann1ModelDir->text();
}

void TrainModelPage::onAnn2SelDataTrainPath()
{
    m_ann2DataPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if ( fileDlg.exec() == QDialog::Accepted )
    {
        QDir dataDir = fileDlg.directory();
        m_ann2DataPath = dataDir.absolutePath();
        ui->ann2DataDir->setText(m_ann2DataPath);
    }
    m_ann2DataPath = ui->ann2DataDir->text();
}

void TrainModelPage::onAnn2SelTestTrainPath()
{
    m_ann2TestPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if ( fileDlg.exec() == QDialog::Accepted )
    {
        QDir dataDir = fileDlg.directory();
        m_ann2TestPath = dataDir.absolutePath();
        ui->ann2TestDir->setText(m_ann2TestPath);
    }
    m_ann2TestPath = ui->ann2TestDir->text();
}

void TrainModelPage::onAnn2SelUndataTrainPath()
{
    m_ann2UndataPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if ( fileDlg.exec() == QDialog::Accepted )
    {
        QDir dataDir = fileDlg.directory();
        m_ann2UndataPath = dataDir.absolutePath();
        ui->ann2UndataDir->setText(m_ann2UndataPath);
    }
    m_ann2UndataPath = ui->ann2UndataDir->text();
}

void TrainModelPage::onAnn2SelUnTestTrainPath()
{
    m_ann2UnTestPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if ( fileDlg.exec() == QDialog::Accepted )
    {
        QDir dataDir = fileDlg.directory();
        m_ann2UnTestPath = dataDir.absolutePath();
        ui->ann2UnTestDir->setText(m_ann2UnTestPath);
    }
    m_ann2UnTestPath = ui->ann2UnTestDir->text();
}

void TrainModelPage::onAnn2SetModelFileName()
{
    m_ann2ModelPath.clear();

    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    if ( fileDlg.exec() == QDialog::Accepted )
    {
        QDir dataDir = fileDlg.directory();
        m_ann1ModelPath = dataDir.absolutePath();
        ui->ann2ModelDir->setText(m_ann2ModelPath);
    }
    m_ann2ModelPath = ui->ann2ModelDir->text();
}
