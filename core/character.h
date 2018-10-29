/**
 * @brief 代码实现引用自easyPR项目
 */
#ifndef CHARACTER_H
#define CHARACTER_H

#include <QtCore>

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "core_global.h"

using namespace cv;

class CORE_EXPORT Character : QObject
{
    Q_OBJECT
public:
    Character(QObject *parent = 0);
    Character(const Character& other);
    ~Character();

    inline void setCharacterMat(Mat param) { m_characterMat = param; }
    inline Mat getCharacterMat() const { return m_characterMat; }

    inline void setCharacterGrayMat(Mat param) { m_characterGrayMat = param; }
    inline Mat getCharacterGrayMat() const { return m_characterGrayMat; }

    inline void setCharacterPos(QRect param) { m_characterPos = param; }
    inline QRect getCharacterPos() const { return m_characterPos; }

    inline void setCharacterStr(QString param) { m_characterStr = param; }
    inline QString getCharacterStr() const { return m_characterStr; }

    inline void setCharacterScore(double param) { m_score = param; }
    inline double getCharacterScore() const { return m_score; }

    inline void setIsChinese(bool param) { m_isChinese = param; }
    inline bool getIsChinese() const { return m_isChinese; }

    inline void setOstuLevel(double param) { m_ostuLevel = param; }
    inline double getOstuLevel() const { return m_ostuLevel; }

    inline void setCenterPoint(QPoint param) { m_center = param; }
    inline QPoint getCenterPoint() const { return m_center; }

    inline void setIndex(int param) { m_index = param; }
    inline int getIndex() const { return m_index; }

    inline bool getIsStrong() const { return m_score >= 0.9; }
    inline bool getIsWeak() const { return m_score < 0.9 && m_score >= 0.5; }
    inline bool getIsLittle() const { return m_score < 0.5; }

    bool operator < (const Character& other) const
    {
      return (m_score < other.m_score);
    }

    bool operator > (const Character& other) const
    {
      return (m_score > other.m_score);
    }

	Character& operator=(const Character& other);

private:

    Mat m_characterMat;      // 字符像素矩阵
    Mat m_characterGrayMat;  // 字符灰度矩阵
    QRect m_characterPos;     // 字符区域
    QString m_characterStr;   // 字符串
    double m_score;          // 比例
    bool m_isChinese;        // 是否是汉字
    double m_ostuLevel;
    QPoint m_center;
    int m_index;

    ////!  m_score >= 0.9
   //bool isStrong;

   ////!  m_score < 0.9 && m_score >= 0.5
   //bool isWeak;

   ////!  m_score < 0.5
   //bool isLittle;
};

#endif // CHARACTER_H
