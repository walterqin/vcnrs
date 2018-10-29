#ifndef TRAINBASE_H
#define TRAINBASE_H

#include <QObject>
#include <opencv2/opencv.hpp>

class TrainBase : public QObject
{
public:
    TrainBase(QObject *parent = 0);
    virtual ~TrainBase();

    virtual void train() = 0;
    virtual void test() = 0;

    void setTrainDataDir(const QString &path) { m_dataDir = path; }
    void setTrainTestDir(const QString &path) { m_testDir = path; }
    void setTrainUndataDir(const QString &path) { m_undataDir = path; }
    void setTrainUnTestDir(const QString &path) { m_unTestDir = path; }
    void setModelDir(const QString &path) { m_modelDir = path; }

private:
    virtual cv::Ptr<cv::ml::TrainData> tdata() = 0;

protected:
    QString m_dataDir;
    QString m_testDir;
    QString m_undataDir;
    QString m_unTestDir;
    QString m_modelDir;

};

#endif // TRAINBASE_H
