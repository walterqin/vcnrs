/**
* @brief 代码来自easyPR项目
*/
#ifndef COREFUNC_H
#define COREFUNC_H

#include "config.h"
#include "numberzone.h"
#include "character.h"
#include "nrm_global.h"

#include <QtCore>

using namespace cv;

namespace corefunc {
    //! find binary image match to color
    NRM_EXPORT Mat colorMatch(const Mat& src, Mat& match, const Color r, const bool adaptive_minsv);

    NRM_EXPORT bool zoneColorJudge(const Mat& src, const Color r, const bool adaptive_minsv, float& percent);

    NRM_EXPORT bool findLeftRightBound(Mat& bound_threshold, int& posLeft, int& posRight);
    NRM_EXPORT bool findLeftRightBound1(Mat& bound_threshold, int& posLeft, int& posRight);
    NRM_EXPORT bool findLeftRightBound2(Mat& bound_threshold, int& posLeft, int& posRight);

    NRM_EXPORT bool clearLiuDing(Mat img);
    NRM_EXPORT void clearLiuDingOnly(Mat& img);
    NRM_EXPORT void clearLiuDing(Mat &mask, int &top, int &bottom);

    NRM_EXPORT Color getPlateType(const Mat& src, const bool adaptive_minsv);

    NRM_EXPORT Mat histeq(Mat in);
    NRM_EXPORT QRect GetCenterRect(Mat& in);
    NRM_EXPORT Mat CutTheRect(Mat& in, QRect& rect);
    NRM_EXPORT int ThresholdOtsu(Mat mat);

    // project histogram
    NRM_EXPORT Mat ProjectedHistogram(Mat img, int t, int threshold = 20);
    NRM_EXPORT Mat showHistogram(const Mat& hist);

    NRM_EXPORT Mat preprocessChar(Mat in, int char_size);

    NRM_EXPORT QRect GetChineseRect(const QRect rectSpe);

    NRM_EXPORT bool verifyCharSizes(QRect r);
    NRM_EXPORT bool verifyPlateSize(QRect mr);
    NRM_EXPORT bool verifyRotatedPlateSizes(RotatedRect mr, bool showDebug = false);

    // non-maximum suppression
    NRM_EXPORT void NMStoCharacter(QVector<Character> &inVec, double overlap);

    // draw rotatedRectangle
    NRM_EXPORT void rotatedRectangle(InputOutputArray img, RotatedRect rrect,
                          const Scalar& color, int thickness = 1,
                          int lineType = LINE_8, int shift = 0);

    // ostu region
    NRM_EXPORT void spatial_ostu(InputArray _src, int grid_x, int grid_y, Color type = BLUE);

    //! use verify size to first generate char candidates
    NRM_EXPORT void mserCharMatch(const Mat &src, std::vector<Mat> &match,
                                   std::vector<NumberZone> &out_plateVec_blue,
                                   std::vector<NumberZone> &out_plateVec_yellow,
                                   bool usePlateMser,
                                   std::vector<RotatedRect>& out_plateRRect_blue,
                                   std::vector<RotatedRect>& out_plateRRect_yellow,
                                   int index = 0, bool showDebug = false);

    // computer the insert over union about two rrect
    NRM_EXPORT bool computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2,
                    const int width, const int height, const float thresh, float& result);
    NRM_EXPORT float computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2,
                     const int width, const int height);

    NRM_EXPORT bool computeIOU(const QRect& rect1, const QRect& rect2, const float thresh, float& result);
    NRM_EXPORT float computeIOU(const QRect& rect1, const QRect& rect2);

    /** @brief convert form mser point to image.*/
    NRM_EXPORT Mat adaptive_image_from_points(const std::vector<Point>& points,
      const Rect& rect, const Size& size, const Scalar& backgroundColor = Scalar(0, 0, 0),
      const Scalar& forgroundColor = Scalar(255, 255, 255), bool gray = true);

    // Calculate a rect have same length and width and remains the center
    NRM_EXPORT QRect adaptive_charrect_from_rect(const Rect& rect, int maxwidth, int maxheight, bool useExtendHeight = false);

    // show detect results
    NRM_EXPORT void showDectectResults(const Mat& img, const QVector<NumberZone> &plateVec, size_t num);

    // show the results
    NRM_EXPORT Mat showResult(const Mat &result, int img_index = 0);

    // enlarge the char rect
    NRM_EXPORT QRectF rectEnlarge(const QRect& src, const int mat_width, const int mat_height);
    NRM_EXPORT QRectF rectFit(const QRect &src, const int mat_width, const int mat_height);

    // write images to temp folder
    NRM_EXPORT void writeTempImage(const Mat& outImg, const QString path, int index = 0);

    // remove small hor lines in the plate
    NRM_EXPORT bool judegMDOratio2(const Mat &image, const Rect &rect, std::vector<Point> &contour, Mat &result,
                        const float thresh = 1.f,
                        bool useExtendHeight = false);

    // clear top and bottom borders
    NRM_EXPORT void clearBorder(const Mat &img, QRect& cropRect);

    NRM_EXPORT void removeOutliers(std::vector<Character> &charGroup, double thresh, Mat result);

    NRM_EXPORT Rect qtRect2CvRect(QRect rect);

    //! non-maximum surpresion for 1d array
    template<typename T>
    void NMSfor1D(const std::vector<T>& arr, std::vector<int>& index) {
        // prepare
        int size = (int)arr.size();
        index.resize(size);
        for (int j = 0; j < size; j++)
            index.at(j) = 0;

        // nms
        int i = 1;
        while (i < size - 1) {
            if (arr.at(i) > arr.at(i + 1)) {
                if (arr.at(i) >= arr.at(i - 1))
                    index.at(i) = 1;
            }
            else {
                while (i < size - 1 && arr.at(i) <= arr.at(i + 1))
                    i = i + 1;
                if (i < size - 1)
                    index.at(i) = 1;
            }
            i = i + 2;
        }
    }
}
#endif // COREFUNC_H

