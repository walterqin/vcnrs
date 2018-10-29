#ifndef CHARSSEGMENT_H
#define CHARSSEGMENT_H

#include <QObject>
#include "config.h"
#include "global.h"

using namespace cv;
using namespace std;
using namespace vc;

class CharsSegment : public QObject
{
public:
    CharsSegment(QObject *parent = 0);

    //! using ostu algotithm the segment chars in number zone
    int charsSegment(Mat input, std::vector<Mat>& resultVec, Color color = BLUE);

    //! using methods to segment chars in number zone
    int charsSegmentUsingOSTU(Mat input, std::vector<Mat>& resultVec, std::vector<Mat>& grayChars, Color color = BLUE);
    int charsSegmentUsingMSER(Mat input, vector<Mat>& resultVec, vector<Mat>& grayChars, Color color = BLUE);

    //! using project
    int projectSegment(const Mat& input, Color color, vector<int>& out_indexs);

    bool verifyCharSizes(Mat r);

    // find the best chinese binaranzation method
    void judgeChinese(Mat in, Mat& out, Color plateType);
    void judgeChineseGray(Mat in, Mat& out, Color plateType);

    Mat preprocessChar(Mat in);

    //! to find the position of chinese
    Rect GetChineseRect(const Rect rectSpe);

    //! find the character refer to city, like "suA" A
    int GetSpecificRect(const std::vector<Rect>& vecRect);

    int RebuildRect(const std::vector<Rect>& vecRect, std::vector<Rect>& outRect,
                    int specIndex);

    int SortRect(const std::vector<Rect>& vecRect, std::vector<Rect>& out);

    inline void setLiuDingSize(int param) { m_LiuDingSize = param; }
    inline void setColorThreshold(int param) { m_ColorThreshold = param; }

    inline void setBluePercent(float param) { m_BluePercent = param; }
    inline float getBluePercent() const { return m_BluePercent; }
    inline void setWhitePercent(float param) { m_WhitePercent = param; }
    inline float getWhitePercent() const { return m_WhitePercent; }

    inline void setDebug(int param) { m_debug = param; }
    inline int getDebug() { return m_debug; }

private:
    int m_LiuDingSize;

    int m_theMatWidth;

    int m_ColorThreshold;
    float m_BluePercent;
    float m_WhitePercent;

    int m_debug;
};
#endif