#include <QString>
#include <QVector>
#include <QMessageBox>
#include <QString>
#include <QStringlist>

#include "trainsvm.h"
#include "util.h"
#include "utilities.h"
#include "config.h"

#ifdef OS_WINDOWS
#include <ctime>
#endif

using namespace cv;
using namespace cv::ml;

TrainSVM::TrainSVM(QString platesFolder, QString xml)
    : TrainBase(0)
    , m_platesFolder(platesFolder)
    , m_svmXml(xml)
{
    assert(!platesFolder.isEmpty());
    assert(!xml.isEmpty());

    extractFeature = getHistomPlusColoFeatures;
}

void TrainSVM::train()
{
    m_svm = cv::ml::SVM::create();
    m_svm->setType(cv::ml::SVM::C_SVC);
    m_svm->setKernel(cv::ml::SVM::RBF);
    m_svm->setDegree(0.1);
    m_svm->setGamma(0.1);
    m_svm->setCoef0(0.1);
    m_svm->setC(1);
    m_svm->setNu(0.1);
    m_svm->setP(0.1);
    m_svm->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 20000, 0.0001));

    this->prepare();

    if (m_trainFileList.isEmpty())
    {
        QString info = QString("No file found in the train folder!\n") +
                    QString("You should create a train data folder.\n") +
                    QString("Copy train data folder(like \"SVM\") under the folder. \n");
        QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), info);
        return;
    }

    cv::Ptr<cv::ml::TrainData> trainData = tdata();
    long start = Utilities::getTimestamp();
    m_svm->trainAuto(trainData, 10, SVM::getDefaultGrid(SVM::C),
                     SVM::getDefaultGrid(SVM::GAMMA), SVM::getDefaultGrid(SVM::P),
                     SVM::getDefaultGrid(SVM::NU), SVM::getDefaultGrid(SVM::COEF),
                     SVM::getDefaultGrid(SVM::DEGREE), true);

    long end = Utilities::getTimestamp();
    int intv = end - start;
    QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),
                             QString("Training done, Time: %1\n").arg(intv));

	m_svm->save(cv::String(m_svmXml.toStdString()));

    this->test();
}

void TrainSVM::test()
{
    LOAD_SVM_MODEL(m_svm, m_svmXml.toStdString());

    if (m_testFileList.isEmpty())
        this->prepare();

    int count_all = m_testFileList.size();
    int ptrue_rtrue = 0;
    int ptrue_rfalse = 0;
    int pfalse_rtrue = 0;
    int pfalse_rfalse = 0;

    foreach(TrainItem item, m_testFileList)
    {
        Mat image = cv::imread(item.file.toStdString());
        if (!image.data)
            continue;

        Mat feature;
        extractFeature(image, feature);

        int predict = (int)(m_svm->predict(feature));
        SvmLabel real = item.label;
        if (predict == kForward && real == kForward) ptrue_rtrue++;
        if (predict == kForward && real == kInverse) ptrue_rfalse++;
        if (predict == kInverse && real == kForward) pfalse_rtrue++;
        if (predict == kInverse && real == kInverse) pfalse_rfalse++;
    }

    double precise = 0;
    if (ptrue_rtrue + ptrue_rfalse != 0)
        precise = ptrue_rtrue / (ptrue_rtrue + ptrue_rfalse);

    double recall = 0;
    if (ptrue_rtrue + pfalse_rtrue != 0)
      recall = ptrue_rtrue / (ptrue_rtrue + pfalse_rtrue);

    double Fsocre = 0;
    if (precise + recall != 0)
      Fsocre = 2 * (precise * recall) / (precise + recall);

    QMessageBox::information(NULL, QString::fromLocal8Bit("信息"),
                             QString("precise: %1, recall: %2, Fscore: %3\n").arg(precise).arg(recall).arg(Fsocre));
}

void TrainSVM::prepare()
{
    srand(unsigned(time(NULL)));

    if (m_dataDir.isEmpty())
        m_dataDir = QString("d:\\has\\train");
    QStringList has_file_train_list = Utilities::getFiles(m_dataDir);
    std::random_shuffle(has_file_train_list.begin(), has_file_train_list.end());

    if (m_testDir.isEmpty())
        m_testDir = QString("d:\\has\\test");
    QStringList has_file_test_list = Utilities::getFiles(m_testDir);
    std::random_shuffle(has_file_test_list.begin(), has_file_test_list.end());

    if (m_undataDir.isEmpty())
        m_undataDir = QString("d:\\no\\train");
    QStringList no_file_train_list = Utilities::getFiles(m_undataDir);
    std::random_shuffle(no_file_train_list.begin(), no_file_train_list.end());

    if (m_unTestDir.isEmpty())
        m_unTestDir = QString("d:\\no\\test");
    QStringList no_file_test_list = Utilities::getFiles(m_unTestDir);
    std::random_shuffle(no_file_test_list.begin(), no_file_test_list.end());

    if (m_modelDir.isEmpty())
        m_modelDir = QString("d:");
    m_svmXml = m_modelDir + QString("\\svm.xml");

    fprintf(stdout, ">> Collecting train data...\n");

    TrainItem ti;
    foreach (QString file, has_file_train_list)
    {
        ti.file = file;
        ti.label = kForward;
        m_trainFileList.push_back(ti);
    }

    foreach (QString file, no_file_train_list)
    {
        ti.file = file;
        ti.label = kInverse;
        m_trainFileList.push_back(ti);
    }

    fprintf(stdout, ">> Collecting test data...\n");

    foreach (QString file, has_file_test_list)
    {
        ti.file = file;
        ti.label = kForward;
        m_testFileList.push_back(ti);
    }

    foreach (QString file, no_file_test_list)
    {
        ti.file = file;
        ti.label = kInverse;
        m_testFileList.push_back(ti);
    }
}

cv::Ptr<cv::ml::TrainData> TrainSVM::tdata()
{
    Mat samples;
    QVector<int> responses;

    foreach(TrainItem item, m_trainFileList)
    {
        Mat image = cv::imread(item.file.toStdString());
        if (!image.data)
            continue;

        Mat feature;
        extractFeature(image, feature);
        feature = feature.reshape(1, 1);

        samples.push_back(feature);
        responses.push_back((int)item.label);
    }

    Mat trainClasses, trainResponses;
    samples.convertTo(trainClasses, CV_32FC1);
    Mat(responses.toStdVector()).copyTo(trainResponses);

    return cv::ml::TrainData::create(trainClasses, cv::ml::SampleTypes::ROW_SAMPLE, trainResponses);
}
