#ifndef ZONEDETECT_H
#define ZONEDETECT_H

#include <QObject>

#include "zonelocate.h"
#include "zonejudge.h"
#include "numberzone.h"
#include "zonedetectinterface.h"

class NRM;

class ZoneDetect : public ZoneDetectInterface
{
    Q_OBJECT
public:
    explicit ZoneDetect(NRM *owner, QObject *parent = 0);
    ~ZoneDetect();

    /** @brief The function detects Number Zone in an image. It can use sobel, color, and character method or the combinations of them.
    *
    * @param src Source image.
    * @param resultVec Destination vector of Number Zone.
    * @param type Detect type. (eg. PR_DETECT_SOBEL + PR_DETECT_COLOR)
    * @param showDetectArea
    * @param index
    */
    int zoneDetect(Mat src, std::vector<NumberZone> &resultVec, int type,
                    bool showDetectArea, int img_index = 0);

    /** @brief
    * The function detects plate in an image. It can use sobel, color, and character method or the combinations of them.
    * Use default m_type, it can use setDetectType() to set it;

    * @param src Source image.
    * @param resultVec Destination vector of CPlate.
    * @param index
    */
    int zoneDetect(Mat src, std::vector<NumberZone> &resultVec, int img_index = 0);

    void LoadSVM(std::string path);

    inline void setPDLifemode(bool param) { m_zoneLocate->setLifemode(param); }
    inline void setDetectType(int param) { m_type = param; }

    inline void setPDDebug(bool param){ m_zoneLocate->setDebug(param); setDetectShow(param);}
    inline bool getPDDebug() { return m_zoneLocate->getDebug(); }

    inline void setGaussianBlurSize(int param) { m_zoneLocate->setGaussianBlurSize(param); }
    inline int getGaussianBlurSize() const { return m_zoneLocate->getGaussianBlurSize(); }

    inline void setMorphSizeWidth(int param) { m_zoneLocate->setMorphSizeWidth(param); }
    inline int getMorphSizeWidth() const { return m_zoneLocate->getMorphSizeWidth(); }

    inline void setMorphSizeHeight(int param) { m_zoneLocate->setMorphSizeHeight(param); }
    inline int getMorphSizeHeight() const { return m_zoneLocate->getMorphSizeHeight(); }

    inline void setVerifyError(float param) {  m_zoneLocate->setVerifyError(param); }
    inline float getVerifyError() const { return m_zoneLocate->getVerifyError(); }

    inline void setVerifyAspect(float param) { m_zoneLocate->setVerifyAspect(param);  }
    inline float getVerifyAspect() const { return m_zoneLocate->getVerifyAspect(); }

    inline void setMaxPlates(int param) { m_maxNumberZone = param; }
    inline int getMaxPlates() const { return m_maxNumberZone; }

    inline void setDetectShow(bool param) { m_showDetect = param; }
    inline bool getDetectShow() const { return m_showDetect; }

    inline void setVerifyMin(int param) { m_zoneLocate->setVerifyMin(param); }
    inline void setVerifyMax(int param) { m_zoneLocate->setVerifyMax(param); }
    inline void setJudgeAngle(int param) { m_zoneLocate->setJudgeAngle(param); }

signals:

public slots:

private:
    int m_maxNumberZone;
    //SceneMode m_mode;
    ZoneLocate* m_zoneLocate;
    int m_type;
    static std::string m_pathSvm;
    bool m_showDetect;  // show the detect result image
};

#endif // ZONEDETECT_H
