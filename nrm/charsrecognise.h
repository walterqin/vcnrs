#ifndef CHARSRECOGNISE_H
#define CHARSRECOGNISE_H

#include <QObject>

#include "character.h"
#include "charsidentify.h"
#include "charssegment.h"
#include "corefunc.h"
#include "numberzone.h"
#include "config.h"
#include "utilities.h"
#include "charsrecogniseinterface.h"

class NRM;

class CharsRecognise : public CharsRecogniseInterface
{
    Q_OBJECT
public:
    explicit CharsRecognise(NRM *owner, QObject *parent = 0);
    ~CharsRecognise();

    int charsRecognise(cv::Mat plate, std::string& plateLicense);
    int charsRecognise(NumberZone& plate, std::string& plateLicense);

    inline std::string getPlateColor(cv::Mat input) const
    {
        std::string color = "未知";
//        Color result = getPlateType(input, true);
//        if (BLUE == result) color = "蓝牌";
//        if (YELLOW == result) color = "黄牌";
//        if (WHITE == result) color = "白牌";
#ifdef OS_WINDOWS
       // color = utils::utf8_to_gbk(color.c_str());
#endif
        return color;
    }

    inline std::string getPlateColor(Color in) const
    {
        std::string color = "未知";
//        if (BLUE == in) color = "蓝牌";
//        if (YELLOW == in) color = "黄牌";
//        if (WHITE == in) color = "白牌";
#ifdef OS_WINDOWS
     //   color = utils::utf8_to_gbk(color.c_str());
#endif
        return color;
    }

    inline void setLiuDingSize(int param) { m_charsSegment->setLiuDingSize(param);}
    inline void setColorThreshold(int param) { m_charsSegment->setColorThreshold(param); }
    inline void setBluePercent(float param) { m_charsSegment->setBluePercent(param); }
    inline float getBluePercent() const { return m_charsSegment->getBluePercent();  }
    inline void setWhitePercent(float param) {  m_charsSegment->setWhitePercent(param); }
    inline float getWhitePercent() const {  return m_charsSegment->getWhitePercent(); }

signals:

public slots:

private:
    CharsSegment* m_charsSegment;
};

#endif // CHARSRECOGNISE_H
