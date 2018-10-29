#include <QString>
#include <QVector>
#include <QMessageBox>
#include <QString>
#include <QStringlist>
#include <numeric>

#include "global.h"
#include "utilities.h"
#include "config.h"
#include "trainann.h"
#include "profile.h"
#include "feature.h"

#ifdef OS_WINDOWS
#include <ctime>
#endif

using namespace cv;
using namespace feature;

TrainANN::TrainANN(QString charFolder, QString xml)
    : TrainBase(0)
    , m_charsFolder(charFolder)
    , m_annXml(xml)
{
    m_ann = cv::ml::ANN_MLP::create();
    type = 0;

}

void TrainANN::train()
{
    int classNumber = 0;
    Mat layers;

    int input_number = 0;
    int hidden_number = 0;
    int output_number = 0;

    if (type == 0)
    {
        classNumber = kCharsTotalNumber;

        input_number = kAnnInput;
        hidden_number = kNeurons;
        output_number = classNumber;
    }
    else if (type == 1)
    {
        classNumber = kChineseNumber;

        input_number = kAnnInput;
        hidden_number = kNeurons;
        output_number = classNumber;
    }

    int N = input_number;
    int m = output_number;
    int first_hidden_neurons = int(std::sqrt((m + 2) * N) + 2 * std::sqrt(N / (m + 2)));
    int second_hidden_neurons = int(m * std::sqrt(N / (m + 2)));

    bool useTLFN = false;
    if (!useTLFN)
    {
        layers.create(1, 3, CV_32SC1);
        layers.at<int>(0) = input_number;
        layers.at<int>(1) = hidden_number;
        layers.at<int>(2) = output_number;
    }
    else
    {
        // Two-layers neural networks is hard to train, So do not try it
//        fprintf(stdout, ">> Use two-layers neural networks,\n");
//        fprintf(stdout, ">> First_hidden_neurons: %d \n", first_hidden_neurons);
//        fprintf(stdout, ">> Second_hidden_neurons: %d \n", second_hidden_neurons);

        layers.create(1, 4, CV_32SC1);
        layers.at<int>(0) = input_number;
        layers.at<int>(1) = first_hidden_neurons;
        layers.at<int>(2) = second_hidden_neurons;
        layers.at<int>(3) = output_number;
    }

    m_ann->setLayerSizes(layers);
    m_ann->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM, 1, 1);
    m_ann->setTrainMethod(cv::ml::ANN_MLP::TrainingMethods::BACKPROP);
    m_ann->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 30000, 0.0001));
    m_ann->setBackpropWeightScale(0.1);
    m_ann->setBackpropMomentumScale(0.1);

    QStringList files = Utilities::getFiles(m_charsFolder);
    if (files.isEmpty())
    {
        QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),
                                 QString("No file found in the train folder!\n"));
        return;
    }

    cv::Ptr<cv::ml::TrainData> trainData = sdata(350);
    long start = Utilities::getTimestamp();
    m_ann->train(trainData);
    long end = Utilities::getTimestamp();
    m_ann->save(m_annXml.toStdString());

    QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),
                             QString("Training done. Time elapse:%1 ms\n").arg(end - start));

    test();
}

void TrainANN::test()
{

}

QPair<QString, QString> TrainANN::identifyChinese(cv::Mat input)
{
	QString s1, s2;
    return qMakePair(s1, s2);
}

QPair<QString, QString> TrainANN::identify(cv::Mat input)
{
    cv::Mat feature = charFeatures2(input, kPredictSize);
    float maxVal = -2.0;
    int result = 0;
    Mat output(1, kCharsTotalNumber, CV_32FC1);
    m_ann->predict(feature, output);

    for (int i = 0; i < kCharsTotalNumber; i++)
    {
        float val = output.at<float>(i);
        if (val > maxVal)
        {
            maxVal = val;
            result = i;
        }
    }

    int index = result;
    if (index < kCharactersNumber)
        return qMakePair(kChars[index], kChars[index]);
    else
    {
        QString key = QString(kChars[index]);
//        QString province = kv->get(key);
//        return qMakePair(key, province);
    }
}

cv::Ptr<cv::ml::TrainData> TrainANN::tdata()
{
    cv::Mat samples;
    QVector<int> labels;

    int classNumber = 0;
    if (type == 0) classNumber = kCharsTotalNumber;
    if (type == 1) classNumber = kChineseNumber;

    for (int i = 0; i < classNumber; i++)
    {
       const char *key = kChars[i + kCharsTotalNumber - classNumber];
       QString subFolder = QString("%1/%2").arg(m_charsFolder).arg(key);
       QStringList fileList = Utilities::getFiles(subFolder);

       foreach(QString file, fileList)
       {
           Mat img = cv::imread(file.toStdString(), 0);
           Mat fps = charFeatures2(img, kPredictSize);
           samples.push_back(fps);
           labels.push_back(i);
       }
    }

    cv::Mat sampleClass;
    samples.convertTo(sampleClass, CV_32F);
    cv::Mat trainClasses = cv::Mat::zeros((int)labels.size(), classNumber, CV_32F);

    for (int i = 0; i < trainClasses.rows; ++i)
        trainClasses.at<float>(i, labels[i]) = 1.0f;

    return cv::ml::TrainData::create(sampleClass, cv::ml::SampleTypes::ROW_SAMPLE,
                                     trainClasses);
}

cv::Mat TrainANN::getSyntheticImage(const Mat &image)
{
    int randType = rand();
    Mat result = image.clone();

    if (randType % 2 == 0)
    {
        int ran_x = rand() % 5 - 2;
        int ran_y = rand() % 5 - 2;

        result = translateImg(result, ran_x, ran_y);
    }
    else
    {
        float angle = float(rand() % 15 - 7);
        result = rotateImg(result, angle);
    }

    return result;
}

cv::Ptr<cv::ml::TrainData> TrainANN::sdata(size_t number_for_count)
{
    cv::Mat samples;
    QVector<int> labels;

    int classNumber = 0;
    if (type == 0) classNumber = kCharsTotalNumber;
    if (type == 1) classNumber = kChineseNumber;

    srand((unsigned)time(0));
    for (int i = 0; i < classNumber; i++)
    {
       const char *key = kChars[i + kCharsTotalNumber - classNumber];
       QString subFolder = QString("%1/%2").arg(m_charsFolder).arg(key);
       QStringList fileList = Utilities::getFiles(subFolder);

       size_t charSize = fileList.size();
       QVector<cv::Mat> matVec;
       matVec.reserve((int)number_for_count);

       foreach(QString file, fileList)
       {
           Mat img = cv::imread(file.toStdString(), 0);
           matVec.push_back(img);
       }

       for (int t = 0; t < (int)number_for_count - (int)charSize; t++)
       {
           int randRange = charSize + t;
           int ranNum = rand() % randRange;
           auto img = matVec.at(ranNum);
           auto simg = getSyntheticImage(img);
           matVec.push_back(simg);
       }

       foreach(cv::Mat img, matVec)
       {
           Mat fps = charFeatures2(img, kPredictSize);
           samples.push_back(fps);
           labels.push_back(i);
       }
    }

    cv::Mat sampleClass;
    samples.convertTo(sampleClass, CV_32F);
    cv::Mat trainClasses = cv::Mat::zeros((int)labels.size(), classNumber, CV_32F);

    for (int i = 0; i < trainClasses.rows; ++i)
        trainClasses.at<float>(i, labels[i]) = 1.0f;

    return cv::ml::TrainData::create(sampleClass, cv::ml::SampleTypes::ROW_SAMPLE,
                                     trainClasses);
}
