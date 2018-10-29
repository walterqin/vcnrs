#ifndef ZONEDETECTINTERFACE_H
#define ZONEDETECTINTERFACE_H

#include <QObject>
#include "core_global.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

using namespace cv;
class NumberZone;
class NumberRecognition;

class CORE_EXPORT ZoneDetectInterface : public QObject
{
    Q_OBJECT
public:
    ZoneDetectInterface(NumberRecognition *ma, QObject *parent = 0);
    ~ZoneDetectInterface();

    NumberRecognition *owner() const { return m_owner; }

    virtual int zoneDectect(Mat src, QVector<NumberZone> &resultVec, int type,
		bool showDetectArea, int img_index = 0) { return 0; }
	virtual int zoneDectect(Mat src, QVector<NumberZone> &resultVec, int img_index = 0) { return 0; }
   // virtual void setDebug(bool flag) = 0;
    virtual void setDetectType(int type) { m_type = type; }

    inline void setDetectShow(bool param) { m_showDetect = param; }
    inline bool getDetectShow() const { return m_showDetect; }

private:
    NumberRecognition *m_owner;
    int m_type;
    bool m_showDetect;  // 是否显示号码区域图像
};

#endif // ZONEDETECTINTERFACE_H
