#ifndef TRAINANNCH_H
#define TRAINANNCH_H

#include "trainbase.h"
#include "feature.h"

using namespace feature;

class TrainAnnCh : public TrainBase
{
public:
    TrainAnnCh(QString charFolder, QString xml);

    virtual void train();
    virtual void test();

    QPair<QString, QString> identifyChinese(cv::Mat input);

    inline void setNumberForCount(int n) { m_numberForCount = n; }

private:
    virtual cv::Ptr<cv::ml::TrainData> tdata();

    void trainVal(size_t number_for_count = 100);

    cv::Ptr<cv::ml::ANN_MLP> m_ann;
    QString m_annXml;
    QString m_charsFolder;
    int type;
    int m_numberForCount;

    annCallback extractFeature;
};

#endif // TRAINANNCH_H
