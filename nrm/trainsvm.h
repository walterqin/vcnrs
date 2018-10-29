#ifndef TRAINSVM_H
#define TRAINSVM_H

#include "trainbase.h"
#include "global.h"
#include "feature.h"

class QString;

using namespace feature;
using namespace vc;

class TrainSVM : public TrainBase
{
public:
    typedef struct
    {
        QString file;
        SvmLabel label;
    }TrainItem;

    TrainSVM(QString platesFolder = QString(), QString xml = QString());

    virtual void train();
    virtual void test();

//    virtual void setTrainDataDir(QString &path);
//    virtual void setTrainTestDir(QString &path);
//    virtual void setTrainNodataDir(QString &path);
//    virtual void setTrainNoTestDir(QString &path);

private:
    void prepare();
    virtual cv::Ptr<cv::ml::TrainData> tdata();

    cv::Ptr<cv::ml::SVM> m_svm;
    QString m_platesFolder;
    QString m_svmXml;
    QVector<TrainItem> m_trainFileList;
    QVector<TrainItem> m_testFileList;

    svmCallback extractFeature;
    bool m_isPrepared = true;
};

#endif // TRAINSVM_H
