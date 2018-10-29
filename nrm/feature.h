#ifndef FEATURE_H_
#define FEATURE_H_

#include "config.h"
#include "nrm_global.h"
#include "global.h"

using namespace cv;

namespace feature {

    //! 获得车牌的特征数
    NRM_EXPORT cv::Mat getHistogram(cv::Mat in);

    //! EasyPR的getFeatures回调函数
    //! 用于从车牌的image生成svm的训练特征features
    typedef void (*svmCallback)(const cv::Mat& image, cv::Mat& features);

    //! EasyPR的getFeatures回调函数
    //! convert from images to features used by gray char ann
    typedef void (*annCallback)(const cv::Mat& image, cv::Mat& features);

    //! gray and project feature
    NRM_EXPORT void getGrayPlusProject(const cv::Mat& grayChar, cv::Mat& features);

    //!  EasyPR的getFeatures回调函数
    //! 本函数是获取垂直和水平的直方图图值
    NRM_EXPORT void getHistogramFeatures(const cv::Mat& image, cv::Mat& features);

    //! 本函数是获取SIFT特征子
    NRM_EXPORT void getSIFTFeatures(const cv::Mat& image, cv::Mat& features);

    //! 本函数是获取HOG特征子
    NRM_EXPORT void getHOGFeatures(const cv::Mat& image, cv::Mat& features);

    //! 本函数是获取HSV空间量化的直方图特征子
    NRM_EXPORT void getHSVHistFeatures(const cv::Mat& image, cv::Mat& features);

    //! LBP feature
    NRM_EXPORT void getLBPFeatures(const cv::Mat& image, cv::Mat& features);

    //! color feature
    NRM_EXPORT void getColorFeatures(const cv::Mat& src, cv::Mat& features);

    //! color feature and histom
    NRM_EXPORT void getHistomPlusColoFeatures(const cv::Mat& image, cv::Mat& features);

    //! get character feature
    NRM_EXPORT cv::Mat charFeatures(cv::Mat in, int sizeData);
    NRM_EXPORT cv::Mat charFeatures2(cv::Mat in, int sizeData);

    //! LBP feature + Histom feature
    NRM_EXPORT void getLBPplusHistFeatures(const cv::Mat& image, cv::Mat& features);

    //! grayChar feauter
    NRM_EXPORT void getGrayCharFeatures(const cv::Mat& grayChar, cv::Mat& features);

    NRM_EXPORT void getGrayPlusLBP(const Mat& grayChar, Mat& features);
}

#endif  // FEATURE_H_
