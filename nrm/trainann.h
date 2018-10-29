#ifndef TRAINANN_H
#define TRAINANN_H

#include "trainbase.h"
#include <QPair>

class TrainANN : public TrainBase
{
public:
    TrainANN(QString charFolder = QString(), QString xml = QString());

    virtual void train();
    virtual void test();

    QPair<QString, QString> identifyChinese(cv::Mat input);
    QPair<QString, QString> identify(cv::Mat input);

private:
    cv::Mat getSyntheticImage(const Mat &image);

    virtual cv::Ptr<cv::ml::TrainData> tdata();
    cv::Ptr<cv::ml::TrainData> sdata(size_t number_for_count = 100);

    cv::Ptr<cv::ml::ANN_MLP> m_ann;
    QString m_annXml;
    QString m_charsFolder;

//    QSharedPointer<Kv> kv_;
    int type;
};

#endif // TRAINANN_H

