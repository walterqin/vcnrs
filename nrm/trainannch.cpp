#include "trainannch.h"

using namespace feature;

TrainAnnCh::TrainAnnCh(QString charFolder, QString xml)
    : TrainBase(0)
    , m_charsFolder(charFolder)
    , m_annXml(xml)
{
    m_ann = cv::ml::ANN_MLP::create();
    type = 1;
    extractFeature = getGrayPlusProject;
}

void TrainAnnCh::train()
{

}

void TrainAnnCh::test()
{

}

QPair<QString, QString> TrainAnnCh::identifyChinese(cv::Mat input)
{
	QString s1, s2;
    return qMakePair(s1, s2);
}

cv::Ptr<cv::ml::TrainData> TrainAnnCh::tdata()
{
    cv::Mat samples;
    cv::Mat trainClasses;
    return cv::ml::TrainData::create(samples, cv::ml::SampleTypes::ROW_SAMPLE,
                                     trainClasses);
}
