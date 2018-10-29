#ifndef ZONEJUDGE_H
#define ZONEJUDGE_H

#include <QObject>
#include <QVector>
#include "config.h"
#include "numberzone.h"
#include "feature.h"
#include "opencv2/ml/ml.hpp"

using namespace cv;
using namespace feature;

class NumberZone;
class QString;

class NRM_EXPORT ZoneJudge : public QObject
{
    Q_OBJECT
public:
	~ZoneJudge();

    static ZoneJudge *instance();

	void loadModel(std::string path);
    int zoneJudgeUsingNMS(std::vector<NumberZone> &res, std::vector<NumberZone> &result, int maxPlates = 5);
    int zoneSetScore(NumberZone &zone);

    int zoneJudge(const Mat &zoneMat);
    int zoneJudge(const std::vector<Mat> &res, std::vector<Mat> &result);
    int zoneJudge(const std::vector<NumberZone> &res, std::vector<NumberZone> &result);


signals:
    void zoneRange(QRect &);

public slots:

private:
    ZoneJudge(QObject *parent = 0);
    static ZoneJudge *m_instance;
    svmCallback extractFeature;
    cv::Ptr<ml::SVM> m_svm;
};

#endif // ZONEJUDGE_H
