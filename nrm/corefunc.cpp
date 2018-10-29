#include <QVector>
#include <QPoint>
#include <QString>

#include "corefunc.h"
#include "charsidentify.h"
#include "mser2.hpp"

using namespace std;
using namespace cv;
namespace corefunc {

Mat colorMatch(const Mat& src, Mat& match, const Color r, const bool adaptive_minsv)
{
    // if use adaptive_minsv
    // min value of s and v is adaptive to h
    const float max_sv = 255;
    const float minref_sv = 64;

    const float minabs_sv = 95; //95;

    // H range of blue

    const int min_blue = 100;  // 100
    const int max_blue = 140;  // 140

    // H range of yellow

    const int min_yellow = 15;  // 15
    const int max_yellow = 40;  // 40

    // H range of white

    const int min_white = 0;   // 15
    const int max_white = 30;  // 40

    Mat src_hsv;

    cvtColor(src, src_hsv, CV_BGR2HSV);

    std::vector<cv::Mat> hsvSplit;
    split(src_hsv, hsvSplit);
    equalizeHist(hsvSplit[2], hsvSplit[2]);
    merge(hsvSplit, src_hsv);

    // match to find the color
    int min_h = 0;
    int max_h = 0;
    switch (r) {
    case BLUE:
        min_h = min_blue;
        max_h = max_blue;
        break;
    case YELLOW:
        min_h = min_yellow;
        max_h = max_yellow;
        break;
    case WHITE:
        min_h = min_white;
        max_h = max_white;
        break;
    default:
        // Color::UNKNOWN
        break;
    }

    float diff_h = float((max_h - min_h) / 2);
    float avg_h = min_h + diff_h;

    int channels = src_hsv.channels();
    int nRows = src_hsv.rows;

    // consider multi channel image
    int nCols = src_hsv.cols * channels;
    if (src_hsv.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }

    int i, j;
    uchar* p;
    float s_all = 0;
    float v_all = 0;
    float count = 0;
    for (i = 0; i < nRows; ++i) {
        p = src_hsv.ptr<uchar>(i);
        for (j = 0; j < nCols; j += 3) {
            int H = int(p[j]);      // 0-180
            int S = int(p[j + 1]);  // 0-255
            int V = int(p[j + 2]);  // 0-255

            s_all += S;
            v_all += V;
            count++;

            bool colorMatched = false;

            if (H > min_h && H < max_h) {
                float Hdiff = 0;
                if (H > avg_h)
                    Hdiff = H - avg_h;
                else
                    Hdiff = avg_h - H;

                float Hdiff_p = float(Hdiff) / diff_h;

                float min_sv = 0;
                if (true == adaptive_minsv)
                    min_sv =
                            minref_sv -
                            minref_sv / 2 *
                            (1
                             - Hdiff_p);  // inref_sv - minref_sv / 2 * (1 - Hdiff_p)
                else
                    min_sv = minabs_sv;  // add

                if ((S > min_sv && S < max_sv) && (V > min_sv && V < max_sv))
                    colorMatched = true;
            }

            if (colorMatched == true) {
                p[j] = 0;
                p[j + 1] = 0;
                p[j + 2] = 255;
            }
            else {
                p[j] = 0;
                p[j + 1] = 0;
                p[j + 2] = 0;
            }
        }
    }

    Mat src_grey;
    std::vector<cv::Mat> hsvSplit_done;
    split(src_hsv, hsvSplit_done);
    src_grey = hsvSplit_done[2];

    match = src_grey;

    return src_grey;
}

bool zoneColorJudge(const Mat& src, const Color r, const bool adaptive_minsv, float& percent)
{
    const float thresh = 0.45f;

    Mat src_gray;
    colorMatch(src, src_gray, r, adaptive_minsv);

    percent =
            float(countNonZero(src_gray)) / float(src_gray.rows * src_gray.cols);
    // cout << "percent:" << percent << endl;

    if (percent > thresh)
        return true;
    else
        return false;
}

bool findLeftRightBound(Mat& bound_threshold, int& posLeft, int& posRight)
{
    float span = bound_threshold.rows * 0.2f;

    for (int i = 0; i < bound_threshold.cols - span - 1; i += 2) {
        int whiteCount = 0;
        for (int k = 0; k < bound_threshold.rows; k++) {
            for (int l = i; l < i + span; l++) {
                if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
                    whiteCount++;
                }
            }
        }
        if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.36) {
            posLeft = i;
            break;
        }
    }
    span = bound_threshold.rows * 0.2f;

    for (int i = bound_threshold.cols - 1; i > span; i -= 2) {
        int whiteCount = 0;
        for (int k = 0; k < bound_threshold.rows; k++) {
            for (int l = i; l > i - span; l--) {
                if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
                    whiteCount++;
                }
            }
        }

        if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.26) {
            posRight = i;
            break;
        }
    }

    if (posLeft < posRight) {
        return true;
    }
    return false;
}

bool findLeftRightBound1(Mat& bound_threshold, int& posLeft, int& posRight)
{
    float span = bound_threshold.rows * 0.2f;

    for (int i = 0; i < bound_threshold.cols - span - 1; i += 3) {
        int whiteCount = 0;
        for (int k = 0; k < bound_threshold.rows; k++) {
            for (int l = i; l < i + span; l++) {
                if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
                    whiteCount++;
                }
            }
        }
        if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.15) {
            posLeft = i;
            break;
        }
    }
    span = bound_threshold.rows * 0.2f;

    for (int i = bound_threshold.cols - 1; i > span; i -= 2) {
        int whiteCount = 0;
        for (int k = 0; k < bound_threshold.rows; k++) {
            for (int l = i; l > i - span; l--) {
                if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
                    whiteCount++;
                }
            }
        }

        if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.06) {
            posRight = i;
            if (posRight + 5 < bound_threshold.cols) {
                posRight = posRight + 5;
            } else {
                posRight = bound_threshold.cols - 1;
            }

            break;
        }
    }

    if (posLeft < posRight) {
        return true;
    }
    return false;
}

bool findLeftRightBound2(Mat& bound_threshold, int& posLeft, int& posRight)
{
    float span = bound_threshold.rows * 0.2f;

    for (int i = 0; i < bound_threshold.cols - span - 1; i += 3) {
        int whiteCount = 0;
        for (int k = 0; k < bound_threshold.rows; k++) {
            for (int l = i; l < i + span; l++) {
                if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
                    whiteCount++;
                }
            }
        }
        if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.32) {
            posLeft = i;
            break;
        }
    }
    span = bound_threshold.rows * 0.2f;


    for (int i = bound_threshold.cols - 1; i > span; i -= 3) {
        int whiteCount = 0;
        for (int k = 0; k < bound_threshold.rows; k++) {
            for (int l = i; l > i - span; l--) {
                if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
                    whiteCount++;
                }
            }
        }

        if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.22) {
            posRight = i;
            break;
        }
    }

    if (posLeft < posRight) {
        return true;
    }
    return false;
}

void clearLiuDingOnly(Mat& img)
{
    const int x = 7;
    Mat jump = Mat::zeros(1, img.rows, CV_32F);
    for (int i = 0; i < img.rows; i++) {
        int jumpCount = 0;
        int whiteCount = 0;
        for (int j = 0; j < img.cols - 1; j++) {
            if (img.at<char>(i, j) != img.at<char>(i, j + 1)) jumpCount++;

            if (img.at<uchar>(i, j) == 255) {
                whiteCount++;
            }
        }

        jump.at<float>(i) = (float) jumpCount;
    }

    for (int i = 0; i < img.rows; i++) {
        if (jump.at<float>(i) <= x) {
            for (int j = 0; j < img.cols; j++) {
                img.at<char>(i, j) = 0;
            }
        }
    }
}

bool clearLiuDing(Mat img)
{
    std::vector<float> fJump;
    int whiteCount = 0;
    const int x = 7;
    Mat jump = Mat::zeros(1, img.rows, CV_32F);
    for (int i = 0; i < img.rows; i++) {
        int jumpCount = 0;

        for (int j = 0; j < img.cols - 1; j++) {
            if (img.at<char>(i, j) != img.at<char>(i, j + 1)) jumpCount++;

            if (img.at<uchar>(i, j) == 255) {
                whiteCount++;
            }
        }

        jump.at<float>(i) = (float) jumpCount;
    }

    int iCount = 0;
    for (int i = 0; i < img.rows; i++) {
        fJump.push_back(jump.at<float>(i));
        if (jump.at<float>(i) >= 16 && jump.at<float>(i) <= 45) {

            // jump condition
            iCount++;
        }
    }

    // if not is not plate
    if (iCount * 1.0 / img.rows <= 0.40) {
        return false;
    }

    if (whiteCount * 1.0 / (img.rows * img.cols) < 0.15 ||
            whiteCount * 1.0 / (img.rows * img.cols) > 0.50) {
        return false;
    }

    for (int i = 0; i < img.rows; i++) {
        if (jump.at<float>(i) <= x) {
            for (int j = 0; j < img.cols; j++) {
                img.at<char>(i, j) = 0;
            }
        }
    }
    return true;
}

void clearLiuDing(Mat &mask, int &top, int &bottom)
{
    const int x = 7;

    for (int i = 0; i < mask.rows / 2; i++) {
        int whiteCount = 0;
        int jumpCount = 0;
        for (int j = 0; j < mask.cols - 1; j++) {
            if (mask.at<char>(i, j) != mask.at<char>(i, j + 1)) jumpCount++;

            if ((int) mask.at<uchar>(i, j) == 255) {
                whiteCount++;
            }
        }
        if ((jumpCount < x && whiteCount * 1.0 / mask.cols > 0.15) ||
                whiteCount < 4) {
            top = i;
        }
    }
    top -= 1;
    if (top < 0) {
        top = 0;
    }

    // ok, find top and bottom boudnadry
    for (int i = mask.rows - 1; i >= mask.rows / 2; i--) {
        int jumpCount = 0;
        int whiteCount = 0;
        for (int j = 0; j < mask.cols - 1; j++) {
            if (mask.at<char>(i, j) != mask.at<char>(i, j + 1)) jumpCount++;
            if (mask.at<uchar>(i, j) == 255) {
                whiteCount++;
            }
        }
        if ((jumpCount < x && whiteCount * 1.0 / mask.cols > 0.15) ||
                whiteCount < 4) {
            bottom = i;
        }
    }
    bottom += 1;
    if (bottom >= mask.rows) {
        bottom = mask.rows - 1;
    }

    if (top >= bottom) {
        top = 0;
        bottom = mask.rows - 1;
    }
}

Color getPlateType(const Mat& src, const bool adaptive_minsv)
{
    float blue_percent = 0;
    float yellow_percent = 0;
    float white_percent = 0;

    if (zoneColorJudge(src, BLUE, adaptive_minsv, blue_percent) == true) {
        // cout << "BLUE" << endl;
        return BLUE;
    } else if (zoneColorJudge(src, YELLOW, adaptive_minsv, yellow_percent) ==
               true) {
        // cout << "YELLOW" << endl;
        return YELLOW;
    } else if (zoneColorJudge(src, WHITE, adaptive_minsv, white_percent) ==
               true) {
        // cout << "WHITE" << endl;
        return WHITE;
    } else {
        //std::cout << "OTHER" << std::endl;

        /*max_percent = blue_percent > yellow_percent ? blue_percent : yellow_percent;
      max_color = blue_percent > yellow_percent ? BLUE : YELLOW;
      max_color = max_percent > white_percent ? max_color : WHITE;*/

        // always return blue
        return BLUE;
    }
}

Mat histeq(Mat in)
{
    Mat out(in.size(), in.type());
    if (in.channels() == 3) {
        Mat hsv;
        std::vector<cv::Mat> hsvSplit;
        cvtColor(in, hsv, CV_BGR2HSV);
        split(hsv, hsvSplit);
        equalizeHist(hsvSplit[2], hsvSplit[2]);
        merge(hsvSplit, hsv);
        cvtColor(hsv, out, CV_HSV2BGR);
    } else if (in.channels() == 1) {
        equalizeHist(in, out);
    }
    return out;
}

#define HORIZONTAL 1
#define VERTICAL 0

QRect GetCenterRect(Mat& in)
{
    int top = 0;
    int bottom = in.rows - 1;

    // find the center rect
    for (int i = 0; i < in.rows; ++i) {
        bool bFind = false;
        for (int j = 0; j < in.cols; ++j) {
            if (in.data[i * in.step[0] + j] > 20) {
                top = i;
                bFind = true;
                break;
            }
        }
        if (bFind) {
            break;
        }
    }

    for (int i = in.rows - 1; i >= 0;  --i) {
        bool bFind = false;
        for (int j = 0; j < in.cols; ++j) {
            if (in.data[i * in.step[0] + j] > 20) {
                bottom = i;
                bFind = true;
                break;
            }
        }
        if (bFind) {
            break;
        }
    }

    int left = 0;
    int right = in.cols - 1;
    for (int j = 0; j < in.cols; ++j) {
        bool bFind = false;
        for (int i = 0; i < in.rows; ++i) {
            if (in.data[i * in.step[0] + j] > 20) {
                left = j;
                bFind = true;
                break;
            }
        }
        if (bFind) {
            break;
        }
    }

    for (int j = in.cols - 1; j >= 0; --j) {
        bool bFind = false;
        for (int i = 0; i < in.rows; ++i) {
            if (in.data[i * in.step[0] + j] > 20) {
                right = j;
                bFind = true;

                break;
            }
        }
        if (bFind) {
            break;
        }
    }

    return QRect(left, top, right - left + 1, bottom - top + 1);
}

Mat CutTheRect(Mat& in, QRect& rect)
{
    int size = in.cols;  // (rect.width>rect.height)?rect.width:rect.height;
    Mat dstMat(size, size, CV_8UC1);
    dstMat.setTo(Scalar(0, 0, 0));

    int x = (int) floor((float) (size - rect.width()) / 2.0f);
    int y = (int) floor((float) (size - rect.height()) / 2.0f);

    for (int i = 0; i < rect.height(); ++i) {

        for (int j = 0; j < rect.width(); ++j) {
            dstMat.data[dstMat.step[0] * (i + y) + j + x] =
                    in.data[in.step[0] * (i + rect.y()) + j + rect.x()];
        }
    }

    //
    return dstMat;
}

float countOfBigValue(Mat &mat, int iValue) {
    float iCount = 0.0;
    if (mat.rows > 1) {
        for (int i = 0; i < mat.rows; ++i) {
            if (mat.data[i * mat.step[0]] > iValue) {
                iCount += 1.0;
            }
        }
        return iCount;

    } else {
        for (int i = 0; i < mat.cols; ++i) {
            if (mat.data[i] > iValue) {
                iCount += 1.0;
            }
        }

        return iCount;
    }
}

int ThresholdOtsu(Mat mat)
{
    int height = mat.rows;
    int width = mat.cols;

    // histogram
    float histogram[256] = {0};
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char p = (unsigned char) ((mat.data[i * mat.step[0] + j]));
            histogram[p]++;
        }
    }
    // normalize histogram
    int size = height * width;
    for (int i = 0; i < 256; i++) {
        histogram[i] = histogram[i] / size;
    }

    // average pixel value
    float avgValue = 0;
    for (int i = 0; i < 256; i++) {
        avgValue += i * histogram[i];
    }

    int thresholdV;
    float maxVariance = 0;
    float w = 0, u = 0;
    for (int i = 0; i < 256; i++) {
        w += histogram[i];
        u += i * histogram[i];

        float t = avgValue * w - u;
        float variance = t * t / (w * (1 - w));
        if (variance > maxVariance) {
            maxVariance = variance;
            thresholdV = i;
        }
    }

    return thresholdV;
}

Mat ProjectedHistogram(Mat img, int t, int threshold)
{
    int sz = (t) ? img.rows : img.cols;
    Mat mhist = Mat::zeros(1, sz, CV_32F);

    for (int j = 0; j < sz; j++) {
        Mat data = (t) ? img.row(j) : img.col(j);

        mhist.at<float>(j) = countOfBigValue(data, threshold);
    }

    // Normalize histogram
    double min, max;
    minMaxLoc(mhist, &min, &max);

    if (max > 0)
        mhist.convertTo(mhist, -1, 1.0f / max, 0);

    return mhist;
}

Mat showHistogram(const Mat& hist)
{
    int height = 32;
    int width = hist.cols;
    Mat show = Mat::zeros(height, width, CV_8UC1);
    for (int i = 0; i < width; i++) {
        int len = int((float) height * hist.at<float>(i));
        for (int j = height - 1; j >= 0; j--) {
            if (height - j <= len)
                show.at<char>(j, i) = (char) 255;
        }
    }
    return show;
}

Mat preprocessChar(Mat in, int char_size)
{
    // Remap image
    int h = in.rows;
    int w = in.cols;

    int charSize = char_size;

    Mat transformMat = Mat::eye(2, 3, CV_32F);
    int m = max(w, h);
    transformMat.at<float>(0, 2) = float(m / 2 - w / 2);
    transformMat.at<float>(1, 2) = float(m / 2 - h / 2);

    Mat warpImage(m, m, in.type());
    warpAffine(in, warpImage, transformMat, warpImage.size(), INTER_LINEAR,
               BORDER_CONSTANT, Scalar(0));

    Mat out;
    cv::resize(warpImage, out, Size(charSize, charSize));

    return out;
}

QRect GetChineseRect(const QRect rectSpe)
{
    int height = rectSpe.height();
    float newwidth = rectSpe.width() * 1.10f;
    int x = rectSpe.x();
    int y = rectSpe.y();

    int newx = x - int(newwidth * 1.10f);
    newx = newx > 0 ? newx : 0;

    return QRect(newx, y, int(newwidth), height);
}

bool verifyCharSizes(QRect r)
{
    // Char sizes 45x90
    float aspect = 45.0f / 90.0f;
    float charAspect = (float) r.width() / (float) r.height();
    float error = 0.35f;
//    float minHeight = 25.f;
//    float maxHeight = 50.f;
    // We have a different aspect ratio for number 1, and it can be ~0.2
    float minAspect = 0.05f;
    float maxAspect = aspect + aspect * error;

    // bb area
    int bbArea = r.width() * r.height();

    if (charAspect > minAspect && charAspect < maxAspect /*&&
                                                               r.rows >= minHeight && r.rows < maxHeight*/)
        return true;
    else
        return false;
}

bool verifyPlateSize(QRect mr)
{
    float error = 0.6f;
    // Spain car plate size: 52x11 aspect 4,7272
    // China car plate size: 440mm*140mm锛宎spect 3.142857

    // Real car plate size: 136 * 32, aspect 4
    float aspect = 3.75;

    // Set a min and max area. All other patchs are discarded
    // int min= 1*aspect*1; // minimum area
    // int max= 2000*aspect*2000; // maximum area
    int min = 34 * 8 * 1;  // minimum area
    int max = 34 * 8 * 200;  // maximum area

    // Get only patchs that match to a respect ratio.
    float rmin = aspect - aspect * error;
    float rmax = aspect + aspect * error;

    float area = float(mr.height() * mr.width());
    float r = (float) mr.width() / (float) mr.height();
    if (r < 1) r = (float) mr.height() / (float) mr.width();

    // cout << "area:" << area << endl;
    // cout << "r:" << r << endl;

    if ((area < min || area > max) || (r < rmin || r > rmax))
      return false;
    else
      return true;
}

bool verifyRotatedPlateSizes(RotatedRect mr, bool showDebug)
{
    float error = 0.65f;
    // Spain car plate size: 52x11 aspect 4,7272
    // China car plate size: 440mm*140mm锛宎spect 3.142857

    // Real car plate size: 136 * 32, aspect 4
    float aspect = 3.75f;

    // Set a min and max area. All other patchs are discarded
    // int min= 1*aspect*1; // minimum area
    // int max= 2000*aspect*2000; // maximum area
    //int min = 34 * 8 * 1;  // minimum area
    //int max = 34 * 8 * 200;  // maximum area

    // Get only patchs that match to a respect ratio.
    float aspect_min = aspect - aspect * error;
    float aspect_max = aspect + aspect * error;

    float width_max = 600.f;
    float width_min = 30.f;

    float min = float(width_min * width_min / aspect_max);  // minimum area
    float max = float(width_max * width_max / aspect_min);  // maximum area

    float width = mr.size.width;
    float height = mr.size.height;
    float area = width * height;

    float ratio = width / height;
    float angle = mr.angle;
    if (ratio < 1) {
        swap(width, height);
        ratio = width / height;

        angle = 90.f + angle;
        //std::cout << "angle:" << angle << std::endl;
    }

    float angle_min = -60.f;
    float angle_max = 60.f;

    //std::cout << "aspect_min:" << aspect_min << std::endl;
    //std::cout << "aspect_max:" << aspect_max << std::endl;

    if (area < min || area > max) {
        if (0 && showDebug) {
            std::cout << "area < min || area > max: " << area << std::endl;
        }

        return false;
    } else if (ratio < aspect_min || ratio > aspect_max) {
        if (0 && showDebug) {
            std::cout << "ratio < aspect_min || ratio > aspect_max: " << ratio << std::endl;
        }

        return false;
    } else if (angle < angle_min || angle > angle_max) {
        if (0 && showDebug) {
            std::cout << "angle < angle_min || angle > angle_max: " << angle << std::endl;
        }

        return false;
    } else if (width < width_min || width > width_max) {
        if (0 && showDebug) {
            std::cout << "width < width_min || width > width_max: " << width << std::endl;
        }

        return false;
    } else {
        return true;
    }

    return true;
}

void NMStoCharacter(QVector<Character> &inVec, double overlap)
{
    qSort(inVec.begin(), inVec.end());

    QVector<Character>::iterator it = inVec.begin();
    for (; it != inVec.end(); ++it) {
        Character charSrc = *it;
        //std::cout << "plateScore:" << plateSrc.getPlateScore() << std::endl;
        QRect rectSrc = charSrc.getCharacterPos();

        QVector<Character>::iterator itc = it + 1;

        for (; itc != inVec.end();) {
            Character charComp = *itc;
            QRect rectComp = charComp.getCharacterPos();
            //Rect rectInter = rectSrc & rectComp;
            //Rect rectUnion = rectSrc | rectComp;
            //double r = double(rectInter.area()) / double(rectUnion.area());

            float iou = computeIOU(rectSrc, rectComp);

            if (iou > overlap) {
                itc = inVec.erase(itc);
            } else {
                ++itc;
            }
        }
    }
}

// judge weather two Character are nearly the same;
static bool compareCharRect(const Character &character1, const Character &character2)
{
    QRect rect1 = character1.getCharacterPos();
    QRect rect2 = character2.getCharacterPos();

    // the character in plate are similar height
    float width_1 = float(rect1.width());
    float height_1 = float(rect1.height());

    float width_2 = float(rect2.width());
    float height_2 = float(rect2.height());

    float height_diff = abs(height_1 - height_2);
    double height_diff_ratio = height_diff / min(height_1, height_2);

    if (height_diff_ratio > 0.25)
        return false;

    // the character in plate are similar in the y-axis
    float y_1 = float(rect1.topLeft().y());
    float y_2 = float(rect2.topLeft().y());

    float y_diff = abs(y_1 - y_2);
    double y_diff_ratio = y_diff / min(height_1, height_2);

    if (y_diff_ratio > 0.5)
        return false;

    // the character center in plate are not to near in the x-axis
    float x_1 = float(rect1.topLeft().x() + rect1.width() / 2);
    float x_2 = float(rect2.topLeft().x() + rect2.width() / 2);

    float x_diff = abs(x_1 - x_2);
    double x_diff_ratio = x_diff / min(height_1, height_2);

    if (x_diff_ratio < 0.25)
        return false;


    // the character in plate are near in the x-axis but not very near
    float x_margin_left = float(min(rect1.bottomRight().x(), rect2.bottomRight().x()));
    float x_margin_right = float(max(rect1.topLeft().x(), rect2.topLeft().x()));

    float x_margin_diff = abs(x_margin_left - x_margin_right);
    double x_margin_diff_ratio = x_margin_diff / min(height_1, height_2);

    if (x_margin_diff_ratio > 1.0)
        return false;

    return true;
}

//! merge chars to group, using the similarity
void mergeCharToGroup(QVector<Character> vecRect, QVector<QVector<Character> > &charGroupVec)
{
    QVector<int> labels;

    int numbers = 0;
    if (vecRect.size() > 0)
        numbers = partition(vecRect.toStdVector(), labels.toStdVector(), &compareCharRect);

    for (size_t j = 0; j < size_t(numbers); j++) {
        QVector<Character> charGroup;

        for (size_t t = 0; t < vecRect.size(); t++) {
            int label = labels[t];

            if (label == j)
                charGroup.push_back(vecRect[t]);
        }

        if (charGroup.size() < 2)
            continue;

        charGroupVec.push_back(charGroup);
    }
}

// draw rotatedRectangle
void rotatedRectangle(InputOutputArray img, RotatedRect rrect,
                      const Scalar& color, int thickness,
                      int lineType, int shift)
{
    Point2f rect_points[4];
    rrect.points(rect_points);
    for (int j = 0; j < 4; j++) {
        cv::line(img, rect_points[j], rect_points[(j + 1) % 4], color, thickness, lineType, shift);
    }
}

void searchWeakSeed(const QVector<Character> &charVec, QVector<Character> &mserCharacter, double thresh1,
                    double thresh2,
                    const Vec4f &line, QPoint &boundaryPoint, const QRect &maxrect, QRect &plateResult, Mat result,
                    CharSearchDirection searchDirection)
{

    float k = line[1] / line[0];
    float x_1 = line[2];
    float y_1 = line[3];

    QVector<Character> searchWeakSeedVec;
    searchWeakSeedVec.reserve(8);

    for (auto weakSeed : charVec) {
        QRect weakRect = weakSeed.getCharacterPos();

        //cv::rectangle(result, weakRect, Scalar(255, 0, 255));

        QPoint weakCenter(weakRect.topLeft().x() + weakRect.width() / 2, weakRect.topLeft().y() + weakRect.height() / 2);
        float x_2 = (float) weakCenter.x();

        if (searchDirection == CharSearchDirection::LEFT) {
            if (weakCenter.x() + weakRect.width() / 2 > boundaryPoint.x()) {
                continue;
            }
        } else if (searchDirection == CharSearchDirection::RIGHT) {
            if (weakCenter.x() - weakRect.width() / 2 < boundaryPoint.x()) {
                continue;
            }
        }

        float y_2l = k * (x_2 - x_1) + y_1;
        float y_2 = (float) weakCenter.y();

        float y_diff_ratio = abs(y_2l - y_2) / maxrect.height();

        if (y_diff_ratio < thresh1) {
            float width_1 = float(maxrect.width());
            float height_1 = float(maxrect.height());

            float width_2 = float(weakRect.width());
            float height_2 = float(weakRect.height());

            float height_diff = abs(height_1 - height_2);
            double height_diff_ratio = height_diff / min(height_1, height_2);

            float width_diff = abs(width_1 - width_2);
            double width_diff_ratio = width_diff / maxrect.width();

            if (height_diff_ratio < thresh1 && width_diff_ratio < 0.5) {
                //std::cout << "h" << height_diff_ratio << std::endl;
                //std::cout << "w" << width_diff_ratio << std::endl;
                searchWeakSeedVec.push_back(weakSeed);
            } else {

            }
        }
    }

    // form right to left to split
    if (searchWeakSeedVec.size() != 0) {
        if (searchDirection == CharSearchDirection::LEFT) {
            std::sort(searchWeakSeedVec.begin(), searchWeakSeedVec.end(),
                      [](const Character &r1, const Character &r2) {
                return r1.getCharacterPos().topLeft().x() > r2.getCharacterPos().topLeft().x();
            });
        } else if (searchDirection == CharSearchDirection::RIGHT) {
            std::sort(searchWeakSeedVec.begin(), searchWeakSeedVec.end(),
                      [](const Character &r1, const Character &r2) {
                return r1.getCharacterPos().topLeft().x() < r2.getCharacterPos().topLeft().x();
            });
        }

        Character firstWeakSeed = searchWeakSeedVec.at(0);
        QRect firstWeakRect = firstWeakSeed.getCharacterPos();
        QPoint firstWeakCenter(firstWeakRect.topLeft().x() + firstWeakRect.width() / 2,
                              firstWeakRect.topLeft().y() + firstWeakRect.height() / 2);

        float ratio = (float) abs(firstWeakCenter.x() - boundaryPoint.x()) / (float) maxrect.height();
        if (ratio > thresh2) {
            if (0) {
                std::cout << "search seed ratio:" << ratio << std::endl;
            }
            return;
        }

        mserCharacter.push_back(firstWeakSeed);
        plateResult |= firstWeakRect;
        boundaryPoint = firstWeakCenter;

        for (size_t weakSeedIndex = 0; weakSeedIndex + 1 < searchWeakSeedVec.size(); weakSeedIndex++) {
            Character weakSeed = searchWeakSeedVec[weakSeedIndex];
            Character weakSeedCompare = searchWeakSeedVec[weakSeedIndex + 1];

            QRect rect1 = weakSeed.getCharacterPos();
            QRect rect2 = weakSeedCompare.getCharacterPos();

            QRect weakRect = rect2;
            QPoint weakCenter(weakRect.topLeft().x() + weakRect.width() / 2, weakRect.topLeft().y() + weakRect.height() / 2);

            // the character in plate are similar height
            float width_1 = float(rect1.width());
            float height_1 = float(rect1.height());

            float width_2 = float(rect2.width());
            float height_2 = float(rect2.height());

            // the character in plate are near in the x-axis but not very near
            float x_margin_left = float(min(rect1.bottomRight().x(), rect2.bottomRight().x()));
            float x_margin_right = float(max(rect1.topLeft().x(), rect2.topLeft().x()));

            float x_margin_diff = abs(x_margin_left - x_margin_right);
            double x_margin_diff_ratio = x_margin_diff / min(height_1, height_2);

            if (x_margin_diff_ratio > thresh2) {
                if (0) {
                    std::cout << "search seed x_margin_diff_ratio:" << x_margin_diff_ratio << std::endl;
                }
                break;
            } else {
                //::rectangle(result, weakRect, Scalar(255, 0, 0), 1);
                mserCharacter.push_back(weakSeedCompare);
                plateResult |= weakRect;
                if (searchDirection == CharSearchDirection::LEFT) {
                    if (weakCenter.x() < boundaryPoint.x()) {
                        boundaryPoint = weakCenter;
                    }
                } else if (searchDirection == CharSearchDirection::RIGHT) {
                    if (weakCenter.x() > boundaryPoint.x()) {
                        boundaryPoint = weakCenter;
                    }
                }
            }
        }
    }
}


void slideWindowSearch(const Mat &image, std::vector<Character> &slideCharacter, const Vec4f &line,
                       Point &fromPoint, const Vec2i &dist, double ostu_level, float ratioWindow,
                       float threshIsCharacter, const Rect &maxrect, Rect &plateResult,
                       CharSearchDirection searchDirection, bool isChinese, Mat &result)
{
    float k = line[1] / line[0];
    float x_1 = line[2];
    float y_1 = line[3];

    int slideLength = int(ratioWindow * maxrect.width);
    int slideStep = 1;
    int fromX = 0;
    if (searchDirection == CharSearchDirection::LEFT) {
        fromX = fromPoint.x - dist[0];
    } else if (searchDirection == CharSearchDirection::RIGHT) {
        fromX = fromPoint.x + dist[0];
    }

    std::vector<Character> charCandidateVec;
    for (int slideX = -slideLength; slideX < slideLength; slideX += slideStep) {
        float x_slide = 0;

        if (searchDirection == CharSearchDirection::LEFT) {
            x_slide = float(fromX - slideX);
        } else if (searchDirection == CharSearchDirection::RIGHT) {
            x_slide = float(fromX + slideX);
        }

        float y_slide = k * (x_slide - x_1) + y_1;
        Point2f p_slide(x_slide, y_slide);
        cv::circle(result, p_slide, 2, Scalar(255, 255, 255), 1);

        int chineseWidth = int(maxrect.width * 1.05);
        int chineseHeight = int(maxrect.height * 1.05);

        Rect rect(Point2f(x_slide - chineseWidth / 2, y_slide - chineseHeight / 2), Size(chineseWidth, chineseHeight));

        if (rect.tl().x < 0 || rect.tl().y < 0 || rect.br().x >= image.cols || rect.br().y >= image.rows)
            continue;

        Mat region = image(rect);
        Mat binary_region;

        cv::threshold(region, binary_region, ostu_level, 255, CV_THRESH_BINARY);
        //double ostu_level = threshold(region, binary_region, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        //std::cout << "ostu_level:" << ostu_level << std::endl;*/

        Mat charInput = preprocessChar(binary_region, 20);

        if (0) {
            imshow("charInput", charInput);
            waitKey(0);
            destroyWindow("charInput");
        }

        Character charCandidate;
        charCandidate.setCharacterPos(QRect(rect.x, rect.y, rect.width, rect.height));
        charCandidate.setCharacterMat(charInput);
        charCandidate.setIsChinese(isChinese);
        charCandidateVec.push_back(charCandidate);
    }

    if (isChinese) {
        CharsIdentify::instance()->classifyChinese(charCandidateVec);
    } else {
        CharsIdentify::instance()->classify(charCandidateVec);
    }

    double overlapThresh = 0.1;
    NMStoCharacter(QVector<Character>::fromStdVector(charCandidateVec), overlapThresh);

    foreach (Character character, charCandidateVec) {
        QRect rect = character.getCharacterPos();
        Point center(rect.topLeft().x() + rect.width() / 2, rect.topLeft().y() + rect.height() / 2);

        if (character.getCharacterScore() > threshIsCharacter && character.getCharacterStr() != "1") {
            //cv::rectangle(result, rect, Scalar(255, 255, 255), 1);
            plateResult |= Rect(rect.x(), rect.y(), rect.width(), rect.height());
            slideCharacter.push_back(character);
            fromPoint = center;

            if (0) {
                std::cout << "label:" << character.getCharacterStr().toStdString();
                std::cout << "__score:" << character.getCharacterScore() << std::endl;
            }
        }
    }
}

static Rect interRect(const Rect &a, const Rect &b)
{
    Rect c;
    int x1 = a.x > b.x ? a.x : b.x;
    int y1 = a.y > b.y ? a.y : b.y;
    c.width = (a.x + a.width < b.x + b.width ? a.x + a.width : b.x + b.width) - x1;
    c.height = (a.y + a.height < b.y + b.height ? a.y + a.height : b.y + b.height) - y1;
    c.x = x1;
    c.y = y1;
    if (c.width <= 0 || c.height <= 0)
        c = Rect();
    return c;
}

static Rect mergeRect(const Rect &a, const Rect &b)
{
    Rect c;
    int x1 = a.x < b.x ? a.x : b.x;
    int y1 = a.y < b.y ? a.y : b.y;
    c.width = (a.x + a.width > b.x + b.width ? a.x + a.width : b.x + b.width) - x1;
    c.height = (a.y + a.height > b.y + b.height ? a.y + a.height : b.y + b.height) - y1;
    c.x = x1;
    c.y = y1;
    return c;
}

// computer the insert over union about two rrect
bool computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2,
                const int width, const int height, const float thresh, float& result)
{
    Rect_<float> safe_rect1;
    calcSafeRect(rrect1, width, height, safe_rect1);

    Rect_<float> safe_rect2;
    calcSafeRect(rrect2, width, height, safe_rect2);

    Rect inter = interRect(safe_rect1, safe_rect2);
    Rect urect = mergeRect(safe_rect1, safe_rect2);

    float iou = (float) inter.area() / (float) urect.area();

    result = iou;

    if (iou > thresh) {
        return true;
    }

    return false;
}

float computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2,
                 const int width, const int height)
{
    Rect_<float> safe_rect1;
    calcSafeRect(rrect1, width, height, safe_rect1);

    Rect_<float> safe_rect2;
    calcSafeRect(rrect2, width, height, safe_rect2);

    Rect inter = interRect(safe_rect1, safe_rect2);
    Rect urect = mergeRect(safe_rect1, safe_rect2);

    float iou = (float) inter.area() / (float) urect.area();
    //std::cout << "iou" << iou << std::endl;

    return iou;
}

bool computeIOU(const QRect& rect1, const QRect& rect2, const float thresh, float& result)
{
    Rect tmp1(rect1.x(), rect1.y(), rect1.width(), rect1.height());
    Rect tmp2(rect2.x(), rect2.y(), rect2.width(), rect2.height());
    Rect inter = interRect(tmp1, tmp2);
    Rect urect = mergeRect(tmp1, tmp2);

    float iou = (float) inter.area() / (float) urect.area();
    result = iou;

    if (iou > thresh) {
        return true;
    }

    return false;
}

float computeIOU(const QRect& rect1, const QRect& rect2)
{
    Rect tmp1(rect1.x(), rect1.y(), rect1.width(), rect1.height());
    Rect tmp2(rect2.x(), rect2.y(), rect2.width(), rect2.height());
    Rect inter = interRect(tmp1, tmp2);
    Rect urect = mergeRect(tmp1, tmp2);

    float iou = (float) inter.area() / (float) urect.area();

    return iou;
}

// the slope are nealy the same along the line
// if one slope is much different others, it should be outliers
// this function to remove it
static void removeRightOutliers(std::vector<Character> &charGroup, std::vector<Character> &out_charGroup, double thresh1,
                           double thresh2, Mat result)
{
      std::sort(charGroup.begin(), charGroup.end(),
                [](const Character &r1, const Character &r2) {
          return r1.getCenterPoint().x() < r2.getCenterPoint().x();
      });

      std::vector<float> slopeVec;
      float slope_last = 0;
      for (size_t charGroup_i = 0; charGroup_i + 1 < charGroup.size(); charGroup_i++) {
          // line_between_two_points
          Vec4f line_btp;
          Character leftChar = charGroup.at(charGroup_i);
          Character rightChar = charGroup.at(charGroup_i + 1);
          QVector<QPoint> two_points;
          two_points.push_back(leftChar.getCenterPoint());
          two_points.push_back(rightChar.getCenterPoint());
          fitLine(Mat(two_points.toStdVector()), line_btp, CV_DIST_L2, 0, 0.01, 0.01);
          float slope = line_btp[1] / line_btp[0];
          slopeVec.push_back(slope);

          if (0) {
              cv::line(result, 
				  Point(leftChar.getCenterPoint().x(), leftChar.getCenterPoint().y()), 
				  Point(rightChar.getCenterPoint().x(), rightChar.getCenterPoint().y()), 
				  Scalar(0, 0, 255));
          }
      }

      int uniformity_count = 0;
      int outlier_index = -1;
      for (size_t slopeVec_i = 0; slopeVec_i + 1 < slopeVec.size(); slopeVec_i++) {
          float slope_1 = slopeVec.at(slopeVec_i);
          float slope_2 = slopeVec.at(slopeVec_i + 1);
          float slope_diff = abs(slope_1 - slope_2);
          if (0) {
              std::cout << "slope_diff:" << slope_diff << std::endl;
          }
          if (slope_diff <= thresh1) {
              uniformity_count++;
          }
          if (0) {
              std::cout << "slope_1:" << slope_1 << std::endl;
              std::cout << "slope_2:" << slope_2 << std::endl;
          }
          if (1/*(slope_1 <= 0 && slope_2 >= 0) || (slope_1 >= 0 && slope_2 <= 0)*/) {
              if (uniformity_count >= 2 && slope_diff >= thresh2) {
                  outlier_index = slopeVec_i + 2;
                  break;
              }
          }
      }
      if (0) {
          std::cout << "uniformity_count:" << uniformity_count << std::endl;
          std::cout << "outlier_index:" << outlier_index << std::endl;
      }

      for (int charGroup_i = 0; charGroup_i < (int) charGroup.size(); charGroup_i++) {
          if (charGroup_i != outlier_index) {
              Character theChar = charGroup.at(charGroup_i);
              out_charGroup.push_back(theChar);
          }
      }

      if (0) {
          std::cout << "end:" << std::endl;
      }
}

static Rect getSafeRect(Point2f center, float width, float height, Mat image)
{
    int rows = image.rows;
    int cols = image.cols;

    float x = center.x;
    float y = center.y;

    float x_tl = (x - width / 2.f);
    float y_tl = (y - height / 2.f);

    float x_br = (x + width / 2.f);
    float y_br = (y + height / 2.f);

    x_tl = x_tl > 0.f ? x_tl : 0.f;
    y_tl = y_tl > 0.f ? y_tl : 0.f;
    x_br = x_br < (float) image.cols ? x_br : (float) image.cols;
    y_br = y_br < (float) image.rows ? y_br : (float) image.rows;

    Rect rect(Point((int) x_tl, int(y_tl)), Point((int) x_br, int(y_br)));
    return rect;
}

// based on the assumptions: distance beween two nearby characters in plate are the same.
// add not found rect and combine two small and near rect.
static void reFoundAndCombineRect(std::vector<Character> &mserCharacter, float min_thresh, float max_thresh,
                             Vec2i dist, Rect maxrect, Mat result)
{
    if (mserCharacter.size() == 0) {
        return;
    }

    std::sort(mserCharacter.begin(), mserCharacter.end(),
              [](const Character &r1, const Character &r2) {
        return r1.getCenterPoint().x() < r2.getCenterPoint().x();
    });

    int comparDist = dist[0] * dist[0] + dist[1] * dist[1];
    if (0) {
        std::cout << "comparDist:" << comparDist << std::endl;
    }

    std::vector<Character> reCharacters;

    size_t mserCharacter_i = 0;
    for (; mserCharacter_i + 1 < mserCharacter.size(); mserCharacter_i++) {
        Character leftChar = mserCharacter.at(mserCharacter_i);
        Character rightChar = mserCharacter.at(mserCharacter_i + 1);

        QPoint leftCenter = leftChar.getCenterPoint();
        QPoint rightCenter = rightChar.getCenterPoint();

        int x_diff = leftCenter.x() - rightCenter.x();
        int y_diff = leftCenter.y() - rightCenter.y();

        // distance between two centers
        int distance2 = x_diff * x_diff + y_diff * y_diff;

        if (0) {
            std::cout << "distance2:" << distance2 << std::endl;
        }

        float ratio = (float) distance2 / (float) comparDist;
        if (ratio > max_thresh) {
            float x_add = (float) (leftCenter.x() + rightCenter.x()) / 2.f;
            float y_add = (float) (leftCenter.y() + rightCenter.y()) / 2.f;

            float width = (float) maxrect.width;
            float height = (float) maxrect.height;

            float x_tl = (x_add - width / 2.f);
            float y_tl = (y_add - height / 2.f);

            //Rect rect_add((int)x_tl, (int)y_tl, (int)width, (int)height);
            Rect rect_add = getSafeRect(Point2f(x_add, y_add), width, height, result);

            reCharacters.push_back(leftChar);

            Character charAdd;
            charAdd.setCenterPoint(QPoint((int) x_add, (int) y_add));
            charAdd.setCharacterPos(QRect(rect_add.x, rect_add.y, rect_add.width, rect_add.height));
            reCharacters.push_back(charAdd);

            if (1) {
                cv::rectangle(result, rect_add, Scalar(0, 128, 255));
            }
        } else if (ratio < min_thresh) {
            QRect rect_union = leftChar.getCharacterPos() | rightChar.getCharacterPos();
            /*float x_add = (float)(leftCenter.x + rightCenter.x) / 2.f;
        float y_add = (float)(leftCenter.y + rightCenter.y) / 2.f;*/
            int x_add = rect_union.topLeft().x() + rect_union.width() / 2;
            int y_add = rect_union.topLeft().y() + rect_union.height() / 2;

            Character charAdd;
            charAdd.setCenterPoint(QPoint(x_add, y_add));
            charAdd.setCharacterPos(rect_union);
            reCharacters.push_back(charAdd);
            if (1) {
                cv::rectangle(result, 
					Rect(rect_union.x(), rect_union.y(), rect_union.width(), rect_union.height()), 
					Scalar(0, 128, 255));
            }

            mserCharacter_i++;
        } else {
            reCharacters.push_back(leftChar);
        }
    }

    if (mserCharacter_i + 1 == mserCharacter.size()) {
        reCharacters.push_back(mserCharacter.at(mserCharacter_i));
    }

    mserCharacter = reCharacters;
}

Rect qtRect2CvRect(QRect rect)
{
    Rect tmp(rect.x(), rect.y(), rect.width(), rect.height());
    return tmp;
}

void removeOutliers(std::vector<Character> &charGroup, double thresh, Mat result)
{
    std::vector<Point> points;
    Vec4f line;
    for (auto character : charGroup) {
        points.push_back(Point(character.getCenterPoint().x(), character.getCenterPoint().y()));
    }

    fitLine(Mat(points), line, CV_DIST_L2, 0, 0.01, 0.01);

    float k = line[1] / line[0];
    float x_1 = line[2];
    float y_1 = line[3];
    float step = 100;
    cv::line(result, Point2f(x_1 - step, y_1 - k * step), Point2f(x_1 + step, k * step + y_1), Scalar(0, 0, 255));

    float a = k;
    float b = -1;
    float c = y_1 - k * x_1;
    float sumdistance = 0;
    for (auto character : charGroup) {
        QPoint center = character.getCenterPoint();
        float distance = (a * center.x() + b * center.y() + c) / std::sqrt(a * a + b * b);
        std::cout << "distance:" << distance << std::endl;
        sumdistance += distance;
    }
    float avgdistance = sumdistance / (float) charGroup.size();

    std::vector<Character>::iterator it = charGroup.begin();
    for (; it != charGroup.end();) {
        QPoint center = it->getCenterPoint();
        float distance = a * center.x() + b * center.y() + c;
        float ratio = distance / avgdistance;
        std::cout << "ratio:" << ratio << std::endl;
        if (ratio > (float) thresh) {
            it = charGroup.erase(it);
        } else {
            ++it;
        }
    }
}

//! use verify size to first generate char candidates
void mserCharMatch(const Mat &src, std::vector<Mat> &match, std::vector<NumberZone> &out_plateVec_blue,
                   std::vector<NumberZone> &out_plateVec_yellow,
                   bool usePlateMser, std::vector<RotatedRect> &out_plateRRect_blue,
                   std::vector<RotatedRect> &out_plateRRect_yellow, int img_index,
                   bool showDebug)
{
    Mat image = src;

    std::vector<std::vector<std::vector<Point>>> all_contours;
    std::vector<std::vector<Rect>> all_boxes;
    all_contours.resize(2);
    all_contours.at(0).reserve(1024);
    all_contours.at(1).reserve(1024);
    all_boxes.resize(2);
    all_boxes.at(0).reserve(1024);
    all_boxes.at(1).reserve(1024);

    match.resize(2);

    std::vector<Color> flags;
    flags.push_back(BLUE);
    flags.push_back(YELLOW);

    const int imageArea = image.rows * image.cols;
    const int delta = 1;
    //const int delta = CParams::instance()->getParam2i();;
    const int minArea = 30;
    const double maxAreaRatio = 0.05;

    Ptr<MSER2> mser;
    mser = MSER2::create(delta, minArea, int(maxAreaRatio * imageArea));
    mser->detectRegions(image, all_contours.at(0), all_boxes.at(0), all_contours.at(1), all_boxes.at(1));

    // mser detect
    // color_index = 0 : mser-, detect white characters, which is in blue plate.
    // color_index = 1 : mser+, detect dark characters, which is in yellow plate.

#pragma omp parallel for
    for (int color_index = 0; color_index < 2; color_index++) {
        Color the_color = flags.at(color_index);

        std::vector<Character> charVec;
        charVec.reserve(128);

        match.at(color_index) = Mat::zeros(image.rows, image.cols, image.type());

        Mat result = image.clone();
        cvtColor(result, result, COLOR_GRAY2BGR);

        size_t size = all_contours.at(color_index).size();

        int char_index = 0;
        int char_size = 20;

        // Chinese plate has max 7 characters.
        const int char_max_count = 7;

        // verify char size and output to rects;
        for (size_t index = 0; index < size; index++) {
            Rect rect = all_boxes.at(color_index)[index];
            std::vector<Point> &contour = all_contours.at(color_index)[index];

            // sometimes a plate could be a mser rect, so we could
            // also use mser algorithm to find plate
            if (usePlateMser) {
                RotatedRect rrect = minAreaRect(Mat(contour));
                if (verifyRotatedPlateSizes(rrect)) {
                    //rotatedRectangle(result, rrect, Scalar(255, 0, 0), 2);
                    if (the_color == BLUE) out_plateRRect_blue.push_back(rrect);
                    if (the_color == YELLOW) out_plateRRect_yellow.push_back(rrect);
                }
            }

            // find character
            if (verifyCharSizes(QRect(rect.x, rect.y, rect.width, rect.height))) {
                Mat mserMat = adaptive_image_from_points(contour, rect, Size(char_size, char_size));
                Mat charInput = preprocessChar(mserMat, char_size);
                Rect charRect = rect;

                Point center(charRect.tl().x + charRect.width / 2, charRect.tl().y + charRect.height / 2);
                Mat tmpMat;
                double ostu_level = cv::threshold(image(charRect), tmpMat, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

                //cv::circle(result, center, 3, Scalar(0, 0, 255), 2);

                // use judegMDOratio2 function to
                // remove the small lines in character like "zh-cuan"
                if (judegMDOratio2(image, rect, contour, result)) {
                    Character charCandidate;
                    charCandidate.setCharacterPos(QRect(charRect.x, charRect.y, charRect.width, charRect.height));
                    charCandidate.setCharacterMat(charInput);
                    charCandidate.setOstuLevel(ostu_level);
                    charCandidate.setCenterPoint(QPoint(center.x, center.y));
                    charCandidate.setIsChinese(false);
                    charVec.push_back(charCandidate);
                }
            }
        }

        // improtant, use matrix multiplication to acclerate the
        // classification of many samples. use the character
        // score, we can use non-maximum superssion (nms) to
        // reduce the characters which are not likely to be true
        // charaters, and use the score to select the strong seed
        // of which the score is larger than 0.9
        CharsIdentify::instance()->classify(charVec);

        // use nms to remove the character are not likely to be true.
        double overlapThresh = 0.6;
        //double overlapThresh = CParams::instance()->getParam1f();
        NMStoCharacter(QVector<Character>::fromStdVector(charVec), overlapThresh);
        charVec.shrink_to_fit();

        std::vector<Character> strongSeedVec;
        strongSeedVec.reserve(64);
        std::vector<Character> weakSeedVec;
        weakSeedVec.reserve(64);
        std::vector<Character> littleSeedVec;
        littleSeedVec.reserve(64);

        //size_t charCan_size = charVec.size();
        for (auto charCandidate : charVec) {
            //CCharacter& charCandidate = charVec[char_index];
            QRect rect = charCandidate.getCharacterPos();
            double score = charCandidate.getCharacterScore();
            if (charCandidate.getIsStrong()) {
                strongSeedVec.push_back(charCandidate);
            } else if (charCandidate.getIsWeak()) {
                weakSeedVec.push_back(charCandidate);
                //cv::rectangle(result, rect, Scalar(255, 0, 255));
            } else if (charCandidate.getIsLittle()) {
                littleSeedVec.push_back(charCandidate);
                //cv::rectangle(result, rect, Scalar(255, 0, 255));
            }
        }

        std::vector<Character> searchCandidate = charVec;

        // nms to srong seed, only leave the strongest one
        overlapThresh = 0.3;
        NMStoCharacter(QVector<Character>::fromStdVector(strongSeedVec), overlapThresh);

        // merge chars to group
        QVector<QVector<Character>> charGroupVec;
        charGroupVec.reserve(64);
        mergeCharToGroup(QVector<Character>::fromStdVector(strongSeedVec), charGroupVec);

        // genenrate the line of the group
        // based on the assumptions , the mser rects which are
        // given high socre by character classifier could be no doubtly
        // be the characters in one plate, and we can use these characeters
        // to fit a line which is the middle line of the plate.
        std::vector<NumberZone> plateVec;
        plateVec.reserve(16);
        for (auto charGroup : charGroupVec) {
            QRect plateResult = charGroup[0].getCharacterPos();
            std::vector<Point> points;
            points.reserve(32);

            Vec4f line;
            int maxarea = 0;
            Rect maxrect;
            double ostu_level_sum = 0;

            int leftx = image.cols;
            Point leftPoint(leftx, 0);
            int rightx = 0;
            Point rightPoint(rightx, 0);

            std::vector<Character> mserCharVec;
            mserCharVec.reserve(32);

            // remove outlier CharGroup
            std::vector<Character> roCharGroup;
            roCharGroup.reserve(32);

            removeRightOutliers(charGroup.toStdVector(), roCharGroup, 0.2, 0.5, result);
            //roCharGroup = charGroup;

            for (auto character : roCharGroup) {
				QRect tmp = character.getCharacterPos();
                Rect charRect(tmp.x(), tmp.y(), tmp.width(), tmp.height());
                cv::rectangle(result, charRect, Scalar(0, 255, 0), 1);
                plateResult |= QRect(charRect.x, charRect.y, charRect.width, charRect.height);

                Point center(charRect.tl().x + charRect.width / 2, charRect.tl().y + charRect.height / 2);
                points.push_back(center);
                mserCharVec.push_back(character);
                //cv::circle(result, center, 3, Scalar(0, 255, 0), 2);

                ostu_level_sum += character.getOstuLevel();

                if (charRect.area() > maxarea) {
                    maxrect = charRect;
                    maxarea = charRect.area();
                }
                if (center.x < leftPoint.x) {
                    leftPoint = center;
                }
                if (center.x > rightPoint.x) {
                    rightPoint = center;
                }
            }

            double ostu_level_avg = ostu_level_sum / (double) roCharGroup.size();
            if (1 && showDebug) {
                std::cout << "ostu_level_avg:" << ostu_level_avg << std::endl;
            }
            float ratio_maxrect = (float) maxrect.width / (float) maxrect.height;

            if (points.size() >= 2 && ratio_maxrect >= 0.3) {
                fitLine(Mat(points), line, CV_DIST_L2, 0, 0.01, 0.01);

                float k = line[1] / line[0];
                //float angle = atan(k) * 180 / (float)CV_PI;
                //std::cout << "k:" << k << std::endl;
                //std::cout << "angle:" << angle << std::endl;
                //std::cout << "cos:" << 0.3 * cos(k) << std::endl;
                //std::cout << "ratio_maxrect:" << ratio_maxrect << std::endl;

                std::sort(mserCharVec.begin(), mserCharVec.end(),
                          [](const Character &r1, const Character &r2) {
                    return r1.getCharacterPos().topLeft().x() < r2.getCharacterPos().topLeft().x();
                });

                Character midChar = mserCharVec.at(int(mserCharVec.size() / 2.f));
                QRect midRect = midChar.getCharacterPos();
                Point midCenter(midRect.topLeft().x() + midRect.width() / 2, midRect.topLeft().y() + midRect.height() / 2);

                int mindist = 7 * maxrect.width;
                std::vector<Vec2i> distVecVec;
                distVecVec.reserve(32);

                Vec2i mindistVec;
                Vec2i avgdistVec;

                // computer the dist which is the distacne between
                // two near characters in the plate, use dist we can
                // judege how to computer the max search range, and choose the
                // best location of the sliding window in the next steps.
                for (size_t mser_i = 0; mser_i + 1 < mserCharVec.size(); mser_i++) {
                    QRect charRect = mserCharVec.at(mser_i).getCharacterPos();
                    Point center(charRect.topLeft().x() + charRect.width() / 2, charRect.topLeft().y() + charRect.height() / 2);

                    QRect charRectCompare = mserCharVec.at(mser_i + 1).getCharacterPos();
                    Point centerCompare(charRectCompare.topLeft().x() + charRectCompare.width() / 2,
                                        charRectCompare.topLeft().y() + charRectCompare.height() / 2);

                    int dist = charRectCompare.x() - charRect.x();
                    Vec2i distVec(charRectCompare.x() - charRect.x(), charRectCompare.y() - charRect.y());
                    distVecVec.push_back(distVec);

                    //if (dist < mindist) {
                    //  mindist = dist;
                    //  mindistVec = distVec;
                    //}
                }

                std::sort(distVecVec.begin(), distVecVec.end(),
                          [](const Vec2i &r1, const Vec2i &r2) {
                    return r1[0] < r2[0];
                });

                avgdistVec = distVecVec.at(int((distVecVec.size() - 1) / 2.f));

                //float step = 10.f * (float)maxrect.width;
                //float step = (float)mindistVec[0];
                float step = (float) avgdistVec[0];

                //cv::line(result, Point2f(line[2] - step, line[3] - k*step), Point2f(line[2] + step, k*step + line[3]), Scalar(255, 255, 255));
                cv::line(result, Point2f(midCenter.x - step, midCenter.y - k * step),
                         Point2f(midCenter.x + step, k * step + midCenter.y), Scalar(255, 255, 255));
                //cv::circle(result, leftPoint, 3, Scalar(0, 0, 255), 2);

                NumberZone plate;
                plate.setNumberZoneLeftPoint(QPoint(leftPoint.x, leftPoint.y));
                plate.setNumberZoneRightPoint(QPoint(rightPoint.x, rightPoint.y));

                plate.setNumberZoneLine(line);
                plate.setNumberZoneDistVec(avgdistVec);
                plate.setOstuLevel(ostu_level_avg);

                plate.setNumberZoneMergeCharRect(Rect(plateResult.x(), plateResult.y(), plateResult.width(), plateResult.height()));
                plate.setNumberZoneMaxCharRect(maxrect);
                plate.setMserCharacter(QVector<Character>::fromStdVector(mserCharVec));
                plateVec.push_back(plate);
            }
        }

        // use strong seed to construct the first shape of the plate,
        // then we need to find characters which are the weak seed.
        // because we use strong seed to build the middle lines of the plate,
        // we can simply use this to consider weak seeds only lie in the
        // near place of the middle line
        for (auto plate : plateVec) {
            Vec4f line = plate.getNumberZoneLine();
            QPoint leftPoint = plate.getNumberZoneLeftPoint();
            QPoint rightPoint = plate.getNumberZoneRightPoint();

            Rect plateResult = plate.getNumberZoneMergeCharRect();
            Rect maxrect = plate.getNumberZoneMaxCharRect();
            Vec2i dist = plate.getNumberZoneDistVec();
            double ostu_level = plate.getOstuLevel();

            std::vector<Character> mserCharacter = plate.getCopyOfMserCharacters().toStdVector();
            mserCharacter.reserve(16);

            float k = line[1] / line[0];
            float x_1 = line[2];
            float y_1 = line[3];

            std::vector<Character> searchWeakSeedVec;
            searchWeakSeedVec.reserve(16);

            std::vector<Character> searchRightWeakSeed;
            searchRightWeakSeed.reserve(8);
            std::vector<Character> searchLeftWeakSeed;
            searchLeftWeakSeed.reserve(8);

            std::vector<Character> slideRightWindow;
            slideRightWindow.reserve(8);
            std::vector<Character> slideLeftWindow;
            slideLeftWindow.reserve(8);

            // draw weak seed and little seed from line;
            // search for mser rect
            if (1 && showDebug) {
                std::cout << "search for mser rect:" << std::endl;
            }

            if (0 && showDebug) {
                std::stringstream ss(std::stringstream::in | std::stringstream::out);
                ss << "resources/image/tmp/" << img_index << "_1_" << "searcgMserRect.jpg";
                imwrite(ss.str(), result);
            }
            if (1 && showDebug) {
                std::cout << "mserCharacter:" << mserCharacter.size() << std::endl;
            }

            // if the count of strong seed is larger than max count, we dont need
            // all the next steps, if not, we first need to search the weak seed in
            // the same line as the strong seed. The judge condition contains the distance
            // between strong seed and weak seed , and the rect simily of each other to improve
            // the roubustnedd of the seed growing algorithm.
            if (mserCharacter.size() < char_max_count) {
                double thresh1 = 0.15;
                double thresh2 = 2.0;
                searchWeakSeed(QVector<Character>::fromStdVector(searchCandidate), 
					QVector<Character>::fromStdVector(searchRightWeakSeed), 
					thresh1, thresh2, line, rightPoint,
					QRect(maxrect.x, maxrect.y, maxrect.width, maxrect.height), 
					QRect(plateResult.x, plateResult.y, plateResult.width, plateResult.height), 
					result, CharSearchDirection::RIGHT);
                if (1 && showDebug) {
                    std::cout << "searchRightWeakSeed:" << searchRightWeakSeed.size() << std::endl;
                }
                for (auto seed : searchRightWeakSeed) {
					QRect tmp = seed.getCharacterPos();
                    cv::rectangle(result, Rect(tmp.x(), tmp.y(), tmp.width(), tmp.height()) , Scalar(255, 0, 0), 1);
                    mserCharacter.push_back(seed);
                }

                searchWeakSeed(QVector<Character>::fromStdVector(searchCandidate), 
					QVector<Character>::fromStdVector(searchLeftWeakSeed), 
					thresh1, thresh2, line, leftPoint,
					QRect(maxrect.x, maxrect.y, maxrect.width, maxrect.height),
					QRect(plateResult.x, plateResult.y, plateResult.width, plateResult.height), 
					result, CharSearchDirection::LEFT);
                if (1 && showDebug) {
                    std::cout << "searchLeftWeakSeed:" << searchLeftWeakSeed.size() << std::endl;
                }
                for (auto seed : searchLeftWeakSeed) {
					QRect tmp = seed.getCharacterPos();
					cv::rectangle(result, Rect(tmp.x(), tmp.y(), tmp.width(), tmp.height()), Scalar(255, 0, 0), 1);
                    mserCharacter.push_back(seed);
                }
            }

            // sometimes the weak seed is in the middle of the strong seed.
            // and sometimes two strong seed are actually the two parts of one character.
            // because we only consider the weak seed in the left and right direction of strong seed.
            // now we examine all the strong seed and weak seed. not only to find the seed in the middle,
            // but also to combine two seed which are parts of one character to one seed.
            // only by this process, we could use the seed count as the condition to judge if or not to use slide window.
            float min_thresh = 0.3f;
            float max_thresh = 2.5f;
            reFoundAndCombineRect(mserCharacter, min_thresh, max_thresh, dist, maxrect, result);

            // if the characters count is less than max count
            // this means the mser rect in the lines are not enough.
            // sometimes there are still some characters could not be captured by mser algorithm,
            // such as blur, low light ,and some chinese characters like zh-cuan.
            // to handle this ,we use a simple slide window method to find them.
            if (mserCharacter.size() < char_max_count) {
                if (1 && showDebug) {
                    std::cout << "search chinese:" << std::endl;
                    std::cout << "judege the left is chinese:" << std::endl;
                }

                // if the left most character is chinese, this means
                // that must be the first character in chinese plate,
                // and we need not to do a slide window to left. So,
                // the first thing is to judge the left charcater is
                // or not the chinese.
                bool leftIsChinese = false;
                if (1) {
                    std::sort(mserCharacter.begin(), mserCharacter.end(),
                              [](const Character &r1, const Character &r2) {
                        return r1.getCharacterPos().topLeft().x() < r2.getCharacterPos().topLeft().x();
                    });

                    Character leftChar = mserCharacter[0];

                    //Rect theRect = adaptive_charrect_from_rect(leftChar.getCharacterPos(), image.cols, image.rows);
                    QRect theRect = leftChar.getCharacterPos();
                    //cv::rectangle(result, theRect, Scalar(255, 0, 0), 1);

                    Mat region = image(Rect(theRect.x(), theRect.y(), theRect.width(), theRect.height()));
                    Mat binary_region;

                    ostu_level = cv::threshold(region, binary_region, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
                    if (1 && showDebug) {
                        std::cout << "left : ostu_level:" << ostu_level << std::endl;
                    }
                    //plate.setOstuLevel(ostu_level);

                    Mat charInput = preprocessChar(binary_region, char_size);
                    if (0 /*&& showDebug*/) {
                        imshow("charInput", charInput);
                        waitKey(0);
                        destroyWindow("charInput");
                    }

                    std::string label = "";
                    float maxVal = -2.f;
                    leftIsChinese = CharsIdentify::instance()->isCharacter(charInput, label, maxVal, true);
                    //auto character = CharsIdentify::instance()->identifyChinese(charInput, maxVal, leftIsChinese);
                    //label = character.second;
                    if (0 /* && showDebug*/) {
                        std::cout << "isChinese:" << leftIsChinese << std::endl;
                        std::cout << "chinese:" << label;
                        std::cout << "__score:" << maxVal << std::endl;
                    }
                }

                // if the left most character is not a chinese,
                // this means we meed to slide a window to find the missed mser rect.
                // search for sliding window
                float ratioWindow = 0.4f;
                //float ratioWindow = CParams::instance()->getParam3f();
                float threshIsCharacter = 0.8f;
                //float threshIsCharacter = CParams::instance()->getParam3f();
                if (!leftIsChinese) {
                    slideWindowSearch(image, slideLeftWindow, line, Point(leftPoint.x(), leftPoint.y()), dist, ostu_level, ratioWindow, threshIsCharacter,
                                      maxrect, plateResult, CharSearchDirection::LEFT, true, result);
                    if (1 && showDebug) {
                        std::cout << "slideLeftWindow:" << slideLeftWindow.size() << std::endl;
                    }
                    for (auto window : slideLeftWindow) {
						QRect tmp = window.getCharacterPos();
						cv::rectangle(result, Rect(tmp.x(), tmp.y(), tmp.width(), tmp.height()), Scalar(255, 0, 0), 1);
                        mserCharacter.push_back(window);
                    }
                }
            }

            // if we still have less than max count characters,
            // we need to slide a window to right to search for the missed mser rect.
            if (mserCharacter.size() < char_max_count) {
                // change ostu_level
                float ratioWindow = 0.4f;
                //float ratioWindow = CParams::instance()->getParam3f();
                float threshIsCharacter = 0.8f;
                //float threshIsCharacter = CParams::instance()->getParam3f();
                slideWindowSearch(image, slideRightWindow, line, Point(rightPoint.x(), rightPoint.y()), dist, plate.getOstuLevel(), ratioWindow,
                                  threshIsCharacter,
                                  maxrect, plateResult, CharSearchDirection::RIGHT, false, result);
                if (1 && showDebug) {
                    std::cout << "slideRightWindow:" << slideRightWindow.size() << std::endl;
                }
                for (auto window : slideRightWindow) {
					QRect tmp = window.getCharacterPos();
					cv::rectangle(result, Rect(tmp.x(), tmp.y(), tmp.width(), tmp.height()), Scalar(255, 0, 0), 1);
                    mserCharacter.push_back(window);
                }
            }

            // computer the plate angle
            float angle = atan(k) * 180 / (float) CV_PI;
            if (1 && showDebug) {
                std::cout << "k:" << k << std::endl;
                std::cout << "angle:" << angle << std::endl;
            }

            // the plateResult rect need to be enlarge to contains all the plate,
            // not only the character area.
            float widthEnlargeRatio = 1.15f; //1.15f;
            float heightEnlargeRatio = 1.25f; //1.25f;
            RotatedRect platePos(
                        Point2f((float) plateResult.x + plateResult.width / 2.f, (float) plateResult.y + plateResult.height / 2.f),
                        Size2f(plateResult.width * widthEnlargeRatio, maxrect.height * heightEnlargeRatio), angle);

            // justify the size is likely to be a plate size.
            if (verifyRotatedPlateSizes(platePos)) {
                rotatedRectangle(result, platePos, Scalar(0, 0, 255), 1);

                plate.setNumberZonePos(platePos);
                plate.setNumberZoneColor(the_color);
                plate.setNumberZoneLocateType(CMSER);

                if (the_color == BLUE) out_plateVec_blue.push_back(plate);
                if (the_color == YELLOW) out_plateVec_yellow.push_back(plate);
            }

            // use deskew to rotate the image, so we need the binary image.
            if (1) {
                for (auto mserChar : mserCharacter) {
					QRect tmp = mserChar.getCharacterPos();
                    match.at(color_index)(Rect(tmp.x(), tmp.y(), tmp.width(), tmp.height())) = 255;
                }
                cv::line(match.at(color_index), Point(rightPoint.x(), rightPoint.y()), 
					Point(leftPoint.x(), leftPoint.y()), Scalar(255));
            }
        }

        if (0 /*&& showDebug*/) {
            imshow("result", result);
            waitKey(0);
            destroyWindow("result");
        }

        if (0) {
            imshow("match", match.at(color_index));
            waitKey(0);
            destroyWindow("match");
        }

        if (1) {
            std::stringstream ss(std::stringstream::in | std::stringstream::out);
            ss << "resources/image/tmp/plateDetect/plate_" << img_index << "_" << the_color << ".jpg";
            imwrite(ss.str(), result);
        }
    }
}

// this spatial_ostu algorithm are robust to
// the plate which has the same light shine, which is that
// the light in the left of the plate is strong than the right.
void spatial_ostu(InputArray _src, int grid_x, int grid_y, Color type)
{
    Mat src = _src.getMat();

    int width = src.cols / grid_x;
    int height = src.rows / grid_y;

    // iterate through grid
    for (int i = 0; i < grid_y; i++) {
        for (int j = 0; j < grid_x; j++) {
            Mat src_cell = Mat(src, Range(i * height, (i + 1) * height), Range(j * width, (j + 1) * width));
            if (type == BLUE) {
                cv::threshold(src_cell, src_cell, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
            } else if (type == YELLOW) {
                cv::threshold(src_cell, src_cell, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
            } else if (type == WHITE) {
                cv::threshold(src_cell, src_cell, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
            } else {
                cv::threshold(src_cell, src_cell, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
            }
        }
    }
}

static bool mat_valid_position(const Mat &mat, int row, int col)
{
    return row >= 0 && col >= 0 && row < mat.rows && col < mat.cols;
}

template<class T>
static void mat_set_invoke(Mat &mat, int row, int col, const Scalar &value)
{
    if (1 == mat.channels()) {
        mat.at<T>(row, col) = (T) value.val[0];
    } else if (3 == mat.channels()) {
        T *ptr_src = mat.ptr<T>(row, col);
        *ptr_src++ = (T) value.val[0];
        *ptr_src++ = (T) value.val[1];
        *ptr_src = (T) value.val[2];
    } else if (4 == mat.channels()) {
        T *ptr_src = mat.ptr<T>(row, col);
        *ptr_src++ = (T) value.val[0];
        *ptr_src++ = (T) value.val[1];
        *ptr_src++ = (T) value.val[2];
        *ptr_src = (T) value.val[3];
    }
}

static void setPoint(Mat &mat, int row, int col, const Scalar &value)
{
    if (CV_8U == mat.depth()) {
        mat_set_invoke<uchar>(mat, row, col, value);
    } else if (CV_8S == mat.depth()) {
        mat_set_invoke<char>(mat, row, col, value);
    } else if (CV_16U == mat.depth()) {
        mat_set_invoke<ushort>(mat, row, col, value);
    } else if (CV_16S == mat.depth()) {
        mat_set_invoke<short>(mat, row, col, value);
    } else if (CV_32S == mat.depth()) {
        mat_set_invoke<int>(mat, row, col, value);
    } else if (CV_32F == mat.depth()) {
        mat_set_invoke<float>(mat, row, col, value);
    } else if (CV_64F == mat.depth()) {
        mat_set_invoke<double>(mat, row, col, value);
    }
}

/** @brief convert form mser point to image.*/
Mat adaptive_image_from_points(const std::vector<Point>& points,
                               const Rect& rect, const Size& size, const Scalar& backgroundColor,
                               const Scalar& forgroundColor, bool gray)
{
    int expendHeight = 0;
    int expendWidth = 0;

    if (rect.width > rect.height) {
        expendHeight = (rect.width - rect.height) / 2;
    } else if (rect.height > rect.width) {
        expendWidth = (rect.height - rect.width) / 2;
    }

    Mat image(rect.height + expendHeight * 2, rect.width + expendWidth * 2, gray ? CV_8UC1 : CV_8UC3, backgroundColor);

    for (int i = 0; i < (int) points.size(); ++i) {
        Point point = points[i];
        Point currentPt(point.x - rect.tl().x + expendWidth, point.y - rect.tl().y + expendHeight);
        if (mat_valid_position(image, currentPt.y, currentPt.x)) {
            setPoint(image, currentPt.y, currentPt.x, forgroundColor);
        }
    }

    Mat result;
    cv::resize(image, result, size, 0, 0, INTER_NEAREST);

    return result;
}

// Calculate a rect have same length and width and remains the center
QRect adaptive_charrect_from_rect(const Rect& rect, int maxwidth, int maxheight, bool useExtendHeight)
{
    int expendWidth = 0;
    int extendHeight = 0;

    if (rect.height > 3 * rect.width) {
        expendWidth = int((int(rect.height * 0.5f) - rect.width) * 0.5f);
        if (useExtendHeight) {
            extendHeight = int(rect.height * 0.3f);
        }
    }

    //Rect resultRect(rect.tl().x - expendWidth, rect.tl().y,
    //  rect.width + expendWidth * 2, rect.height);

    int tlx = rect.tl().x - expendWidth > 0 ? rect.tl().x - expendWidth : 0;
    int tly = rect.tl().y - extendHeight > 0 ? rect.tl().y - extendHeight : 0;

    int brx = rect.br().x + expendWidth < maxwidth ? rect.br().x + expendWidth : maxwidth;
    int bry = rect.br().y + extendHeight < maxheight ? rect.br().y + extendHeight : maxheight;

    QRect resultRect(tlx, tly, brx - tlx, bry - tly);
    return resultRect;
}



// show detect results
void showDectectResults(const Mat& img, const QVector<NumberZone> &plateVec, size_t num)
{
    int index = 0;
    if (1) {
        Mat result;
        img.copyTo(result);
        for (size_t j = 0; j < plateVec.size(); j++) {
            // add plates to left corner
            const NumberZone& item = plateVec.at(j);
            Mat plateMat = item.getNumberZoneMat();

            int height = 36;
            int width = 136;
            if (height * index + height < result.rows) {
                Mat imageRoi = result(Rect(0, 0 + height * index, width, height));
                addWeighted(imageRoi, 0, plateMat, 1, 0, imageRoi);
            }
            index++;

            // draw the bouding box
            RotatedRect theRect = item.getNumberZonePos();
            float scale = item.getNumberZoneScale();
            RotatedRect minRect = scaleBackRRect(theRect, scale);

            Point2f rect_points[4];
            minRect.points(rect_points);
            Scalar lineColor = Scalar(255, 255, 255);
            if (item.getNumberZoneLocateType() == SOBEL) lineColor = Scalar(255, 0, 0);
            if (item.getNumberZoneLocateType() == COLOR) lineColor = Scalar(0, 255, 0);
            if (item.getNumberZoneLocateType() == CMSER) lineColor = Scalar(0, 0, 255);

            for (int j = 0; j < 4; j++)
                line(result, rect_points[j], rect_points[(j + 1) % 4], lineColor, 2, 8);
        }
        showResult(result);
    }
}

// show the results
Mat showResult(const Mat &result, int img_index)
{
    namedWindow("CNR", CV_WINDOW_AUTOSIZE);

    const int RESULTWIDTH = kShowWindowWidth;   // 640 930
    const int RESULTHEIGHT = kShowWindowHeight;  // 540 710

    Mat img_window;
    img_window.create(RESULTHEIGHT, RESULTWIDTH, CV_8UC3);

    int nRows = result.rows;
    int nCols = result.cols;

    Mat result_resize;
    if (nCols <= img_window.cols && nRows <= img_window.rows) {
        result_resize = result;

    } else if (nCols > img_window.cols && nRows <= img_window.rows) {
        float scale = float(img_window.cols) / float(nCols);
        resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);

    } else if (nCols <= img_window.cols && nRows > img_window.rows) {
        float scale = float(img_window.rows) / float(nRows);
        resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);

    } else if (nCols > img_window.cols && nRows > img_window.rows) {
        float scale1 = float(img_window.cols) / float(nCols);
        float scale2 = float(img_window.rows) / float(nRows);
        float scale = scale1 < scale2 ? scale1 : scale2;
        resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);
    }
    else {
        result_resize = result;
    }

    Mat imageRoi = img_window(Rect((RESULTWIDTH - result_resize.cols) / 2,
                                   (RESULTHEIGHT - result_resize.rows) / 2,
                                   result_resize.cols, result_resize.rows));
    addWeighted(imageRoi, 0, result_resize, 1, 0, imageRoi);

    if (1) {
        imshow("EasyPR", img_window);
        waitKey(0);
        destroyWindow("EasyPR");
    }

    if (1) {
        std::stringstream ss(std::stringstream::in | std::stringstream::out);
        ss << "resources/image/tmp/Result/plate_" << img_index << ".jpg";
        imwrite(ss.str(), img_window);
    }

    return img_window;
}

// enlarge the char rect
QRectF rectEnlarge(const QRect& src, const int mat_width, const int mat_height)
{
    float w = (float) src.width();
    float h = (float) src.height();
    // enlarge the rect,
    // width to 120%
    // height to 105%
    float new_w = w * 1.2f;
    float new_h = h * 1.05f;

    Rect_<float> boudRect;
    boudRect.x = (float) src.x() - w * 0.1f;
    boudRect.y = (float) src.y() - h * 0.025f;
    boudRect.width = new_w;
    boudRect.height = new_h;

    float tl_x = boudRect.x > 0 ? boudRect.x : 0;
    float tl_y = boudRect.y > 0 ? boudRect.y : 0;

    float br_x = boudRect.x + boudRect.width - 1 <= mat_width - 1
            ? boudRect.x + boudRect.width - 1
            : mat_width - 1;
    float br_y = boudRect.y + boudRect.height - 1 < mat_height - 1
            ? boudRect.y + boudRect.height - 1
            : mat_height - 1;

    float roi_width = br_x - tl_x + 1;
    float roi_height = br_y - tl_y + 1;

	if (roi_width <= 0 || roi_height <= 0)
        return QRectF(src);

    //! a new rect not out the range of mat
    return QRectF(tl_x, tl_y, roi_width, roi_height);
}

QRectF rectFit(const QRect &src, const int mat_width, const int mat_height)
{
    float w = (float)src.width();
    float h = (float)src.height();
    float new_w = h * 0.5f;
    float new_h = h * 1.05f;

    if (new_w <= w || new_h <= h) {
        return QRectF(src);
    }

    float ex_w = (new_w - w) * 0.5f;
    float ex_h = (new_h - h) * 0.5f;

    Rect_<float> boudRect;
    boudRect.x = (float)src.x() - ex_w;
    boudRect.y = (float)src.y() - ex_h;
    boudRect.width = new_w;
    boudRect.height = new_h;

    float tl_x = boudRect.x > 0 ? boudRect.x : 0;
    float tl_y = boudRect.y > 0 ? boudRect.y : 0;

    float br_x = boudRect.x + boudRect.width - 1 <= mat_width - 1
            ? boudRect.x + boudRect.width - 1
            : mat_width - 1;
    float br_y = boudRect.y + boudRect.height - 1 < mat_height - 1
            ? boudRect.y + boudRect.height - 1
            : mat_height - 1;

    float roi_width = br_x - tl_x + 1;
    float roi_height = br_y - tl_y + 1;

    if (roi_width <= 2 || roi_height <= 2)
        return QRectF(src);

    //! a new rect not out the range of mat
    return QRectF(tl_x, tl_y, roi_width - 1, roi_height - 1);
}

// write images to temp folder
void writeTempImage(const Mat& outImg, const QString path, int index)
{
    time_t t = time(0);  // get time now
    struct tm *now = localtime(&t);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H_%M_%S", now);
    QString ss;
    ss = QString("resources/image/tmp/") + path + QString("_") + QString(buf) + QString("_") + index + QString(".jpg");
    imwrite(String(ss.toStdString().data()), outImg);
}

// remove small hor lines in the plate
bool judegMDOratio2(const Mat &image, const Rect &rect, std::vector<Point> &contour, Mat &result,
                    const float thresh,  bool useExtendHeight)
{
    Mat mser = image(rect);
    Mat mser_mat;
    cv::threshold(mser, mser_mat, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	QRect tmp = adaptive_charrect_from_rect(rect, image.cols, image.rows, useExtendHeight);
	Rect normalRect = Rect(tmp.x(), tmp.y(), tmp.width(), tmp.height());
    Mat region = image(normalRect);
    Mat thresh_mat;
    cv::threshold(region, thresh_mat, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    // count mser diff ratio
    int countdiff = abs(countNonZero(thresh_mat) - countNonZero(mser_mat));

    float MserDiffOstuRatio = float(countdiff) / float(rect.area());

    if (MserDiffOstuRatio > thresh) {
        //std::cout << "MserDiffOstuRatio:" << MserDiffOstuRatio << std::endl;
        /*imshow("tmpMat", mser_mat);
      waitKey(0);
      imshow("tmpMat", thresh_mat);
      waitKey(0);*/

        cv::rectangle(result, normalRect, Scalar(0, 0, 0), 2);
        return false;
    }

    return true;
}

// clear top and bottom borders
void clearBorder(const Mat &img, QRect& cropRect)
{

}

}//namespace corefunc
