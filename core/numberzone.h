/**
* @brief 代码引用自esayPR项目
*/
#ifndef NUMBERZONE_H
#define NUMBERZONE_H

#include <QObject>
#include "core_global.h"
#include "character.h"
#include "global.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

using namespace vc;

class QString;
class QPoint;

class CORE_EXPORT NumberZone : public QObject
{
    Q_OBJECT
public:
    NumberZone(QObject *parent = 0);
    NumberZone(const NumberZone &other);
    ~NumberZone();

    NumberZone& operator=(const NumberZone& other);

    inline void setNumberZoneMat(Mat param) { m_numberZoneMat = param; }
    inline Mat getNumberZoneMat() const { return m_numberZoneMat; }

    inline void setChineseMat(Mat param) { m_chineseMat = param; }
    inline Mat getChineseMat() const { return m_chineseMat; }

    inline void setChineseKey(QString param) { m_chineseKey = param; }
    inline QString getChineseKey() const { return m_chineseKey; }

    inline void setNumberZonePos(RotatedRect param) { m_numberZonePos = param; }
    inline RotatedRect getNumberZonePos() const { return m_numberZonePos; }

    inline void setNumberZoneStr(QString param) { m_numberZoneStr = param; }
    inline QString getNumberZoneStr() const { return m_numberZoneStr; }

    inline void setNumberZoneLocateType(LocateType param) { m_locateType = param; }
    inline LocateType getNumberZoneLocateType() const { return m_locateType; }

    inline void setNumberZoneColor(Color param) { m_numberZoneColor = param; }
    inline Color getNumberZoneColor() const { return m_numberZoneColor; }

    inline void setNumberZoneScale(float param) { m_scale = param; }
    inline float getNumberZoneScale() const { return m_scale; }

    inline void setNumberZoneScore(double param) { m_score = param; }
    inline double getNumberZoneScore() const { return m_score; }

    inline void setNumberZoneLine(Vec4f param) { m_line = param; }
    inline Vec4f getNumberZoneLine() const { return m_line; }

    inline void setNumberZoneLeftPoint(QPoint param) { m_leftPoint = param; }
    inline QPoint getNumberZoneLeftPoint() const { return m_leftPoint; }

    inline void setNumberZoneRightPoint(QPoint param) { m_rightPoint = param; }
    inline QPoint getNumberZoneRightPoint() const { return m_rightPoint; }

    inline void setNumberZoneMergeCharRect(Rect param) { m_mergeCharRect = param; }
    inline Rect getNumberZoneMergeCharRect() const { return m_mergeCharRect; }

    inline void setNumberZoneMaxCharRect(Rect param) { m_maxCharRect = param; }
    inline Rect getNumberZoneMaxCharRect() const { return m_maxCharRect; }

    inline void setNumberZoneDistVec(Vec2i param) { m_distVec = param; }
    inline Vec2i getNumberZoneDistVec() const { return m_distVec; }

    inline void setOstuLevel(double param) { m_ostuLevel = param; }
    inline double getOstuLevel() const { return m_ostuLevel; }

    inline void setMserCharacter(const QVector<Character>& param) { m_mserCharVec = param; }
    inline void addMserCharacter(Character param) { m_mserCharVec.push_back(param); }
    inline QVector<Character> getCopyOfMserCharacters() { return m_mserCharVec; }

    inline void setReutCharacter(const QVector<Character>& param) { m_reutCharVec = param; }
    inline void addReutCharacter(Character param) { m_reutCharVec.push_back(param); }
    inline QVector<Character> getCopyOfReutCharacters() { return m_reutCharVec; }

    bool operator < (const NumberZone& plate) const { return (m_score < plate.m_score); }
    bool operator < (const NumberZone& plate) { return (m_score < plate.m_score); }

signals:

public slots:

private:

    Mat m_numberZoneMat;
    RotatedRect m_numberZonePos;
    QString m_numberZoneStr;
    LocateType m_locateType;
    Color m_numberZoneColor;
    float m_scale;
    double m_score;
    double m_ostuLevel;
    Vec4f m_line;
    QPoint m_leftPoint;
    QPoint m_rightPoint;

    Rect m_mergeCharRect;
    Rect m_maxCharRect;

    QVector<Character> m_mserCharVec;
    QVector<Character> m_slwdCharVec;

    QVector<Character> m_ostuCharVec;
    QVector<Character> m_reutCharVec;

    int m_charCount;
    Mat m_chineseMat;
    QString m_chineseKey;
    Vec2i m_distVec;
};

#endif // NUMBERZONE_H
