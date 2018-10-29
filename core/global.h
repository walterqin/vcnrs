/**
* @file global.h
* @brief 定义项目中用到的全局数据，包括：
*        全局常数
*        通用API
*        全局变量
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-07
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>
#include <QMessageBox>
#include <QMetaType>
#include <math.h>
#include <QtDebug>

#include "core_global.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
class QSqlRecord;

#define DATABASEPATH ""

static const int   kShowWindowWidth = 1000;
static const int   kShowWindowHeight = 800;

static const int   kPlateResizeWidth = 136;
static const int   kPlateResizeHeight = 36;

static const int   kCharacterInput  = 120;
static const int   kChineseInput    = 440;
static const int   kAnnInput = kCharacterInput;

static const int   kCharacterSize = 10;
static const int   kChineseSize = 20;
static const int   kPredictSize = kCharacterSize;

namespace vc
{
    /*分析状态*/
    enum AnalysisStatus
    {
        Ready,          //准备
        Reprocess,      //预处理
        Analysing,      //分析
        Finished,       //完成
        Validated,      //确认
    };

    /*! 分析优先级 */
    enum AnalysisPriority
    {
        HighPriority,
        NormalPriority,
        LowPriority,
    };

    enum CameraNo
    {
        CameraOne = 0,
        CameraTwo,
        CameraThree,
        CameraFour,
        CameraCnt,
    };

    enum Color { BLUE, YELLOW, WHITE, UNKNOWN };

    enum LocateType { SOBEL, COLOR, CMSER, OTHER };

    enum CharSearchDirection { LEFT, RIGHT };

    enum SvmLabel{
      kForward = 1, // correspond to "has plate"
      kInverse = 0  // correspond to "no plate"
    };

    enum PR_MODE
    {
        PR_MODE_UNCONSTRAINED,
        PR_MODE_CAMERPOHNE,
        PR_MODE_PARKING,
        PR_MODE_HIGHWAY
    };

    enum PR_DETECT
    {
        PR_DETECT_SOBEL = 0x01,  /**Sobel detect type, using twice Sobel  */
        PR_DETECT_COLOR = 0x02,  /**Color detect type   */
        PR_DETECT_CMSER = 0x04,  /**Character detect type, using mser  */
    };

    enum ResultFlag
    {
        ReplicationDiff			= 0x0400,	// 多次测量结果不一致
        EvolveResult			= 0x0800,	// 演算的结果
        ManualTestResult		= 0x1000,	// 手动测试结果
        ManualInputResult		= 0x8000,	// 用户输入的结果
    };
    Q_DECLARE_FLAGS(ResultFlags, ResultFlag)
}
Q_DECLARE_METATYPE(vc::AnalysisStatus)
Q_DECLARE_METATYPE(vc::Color)


#define _STR2(x) #x
#define _STR(x) _STR2(x)
#define FILE_POSITION __FILE__"("_STR(__LINE__)"):"

#define TRACE_LAST_ERROR(q) qWarning() << FILE_POSITION << (q).lastError()

#ifndef _countof
#define _countof(a) (sizeof(a)/sizeof((a)[0]))
#endif

#ifdef QT_NO_DEBUG
    #define QDEBUG(x)		qt_noop()
    #define QWARNING(x)		qt_noop()
#else
    #define QDEBUG(x)		qDebug() << x
    #define QWARNING(x)		qWarning() << __FILE__ << "(" << __LINE__ << "):" << x
#endif

#define PRINTABLE(s)		QString(s).toUtf8().constData()

inline int qcPackStopFlag(int flags) { return flags << 16; }
inline int qcExtractStopFlag(int flags) { return flags >> 16; }

CORE_EXPORT double fraction(const QString &expr, bool *ok = 0);
CORE_EXPORT QVariant eval(const QString &expr);
CORE_EXPORT double round(double val, int precision = 4, char format = 'g');
CORE_EXPORT void sleep(int ms);
CORE_EXPORT int nextSampleNo(const QDate &date);
CORE_EXPORT void setGeneratedValue(QSqlRecord &rec, const QString &field, const QVariant &value);
CORE_EXPORT unsigned short crc16(const void *buf, int len);

// uniform resize all the image to same size for the next process
CORE_EXPORT Mat uniformResize(const Mat &result, float& scale);
// Scale back RotatedRect
CORE_EXPORT RotatedRect scaleBackRRect(const RotatedRect& rr, const float scale_ratio);
// Scale to small image (for the purpose of comput mser in large image)
CORE_EXPORT Mat scaleImage(const Mat& image, const Size& maxSize, double& scale_ratio);
// calc safe rect
CORE_EXPORT bool calcSafeRect(const RotatedRect& roi_rect, const Mat& src, Rect_<float>& safeBoundRect);
CORE_EXPORT bool calcSafeRect(const RotatedRect &roi_rect, const int width, const int height, Rect_<float> &safeBoundRect);

// uniform resize all the plates to same size for the next process
CORE_EXPORT Mat uniformResizePlates(const Mat &result, float& scale);


template<typename T> T square(T t) { return t*t; }
template<typename T> const T &clamp(const T &val, const T &min, const T &max)
{ return val < min ? min : val > max ? max : val; }	// see qBound
template<typename T> void qDelete(T &p) { if (p) { delete p; p = 0; } }
template<typename T> int sign(const T &t) { return t >= 0 ? 1 : -1; }

// 计算样本偏差和平均值
template<typename T>
double stdev(const T *dat, int n, double *mean = 0)
{
    double m = 0;

    for(int i=0; i<n; i++)
        m += dat[i];
    m /= n;
    if (mean)
        *mean = m;

    double s = 0;

    for(int i=0; i<n; i++)
        s += square(dat[i] - m);
    s /= n;// - 1;
    s = sqrt(s);

    return s;
}

// 不确定度计算
template<typename T>
double uncertainty(const T *dat, int n, double *mean = 0)
{
    double m = 0;

    for(int i=0; i<n; i++)
        m += dat[i];
    m /= n;
    if (mean)
        *mean = m;

    double s = 0;

    for(int i=0; i<n; i++)
        s += square(dat[i] - m);
    s /= n - 1;
    s = sqrt(s);

    return s / qAbs(m);
}

 QWidget *topLevelWidget();

//信息提示函数
CORE_EXPORT QMessageBox::StandardButton
information(const QString &text,
            QMessageBox::StandardButtons buttons = QMessageBox::Ok,
            QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

CORE_EXPORT QMessageBox::StandardButton
question(const QString &text,
         QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No,
         QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

CORE_EXPORT QMessageBox::StandardButton
warning(const QString &text,
        QMessageBox::StandardButtons buttons = QMessageBox::Ok,
        QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

CORE_EXPORT QMessageBox::StandardButton
critical(const QString &text,
         QMessageBox::StandardButtons buttons = QMessageBox::Ok,
         QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

template<class T>
class VariableHolder
{
public:
    VariableHolder(T &var, const T &val = T()) : m_var(var), m_val(var)
    {
        m_var = val;
    }
    ~VariableHolder()
    {
        restore();
    }
    void restore()
    {
        m_var = m_val;
    }

    T & m_var;
    const T m_val;
};

class CORE_EXPORT BoolLocker
{
public:
    BoolLocker(bool &b, bool val = true) : m_b(b)
    {
        m_val = m_b;
        m_b = val;
    }
    ~BoolLocker() { m_b = m_val; }
    bool &m_b;
    bool m_val;
};

class CORE_EXPORT WaitCursor
{
public:
    WaitCursor(Qt::CursorShape cursor = Qt::WaitCursor);
    ~WaitCursor();
};

CORE_EXPORT Mat translateImg(Mat img, int offsetx, int offsety, int bk = 0);
// rotate an image
CORE_EXPORT Mat rotateImg(Mat source, float angle, int bk = 0);

// crop the image
CORE_EXPORT Mat cropImg(Mat src, int x, int y, int shift, int bk = 0);

CORE_EXPORT Mat generateSyntheticImage(const Mat& image, int use_swap = 1);

/////////////////////////////////////////////////////////////////////
///
static const char *kChars[] = {
    "0", "1", "2",
    "3", "4", "5",
    "6", "7", "8",
    "9",
    /*  10  */
    "A", "B", "C",
    "D", "E", "F",
    "G", "H", "I"
    "J", "K", "L",
    "M", "N", "O"
    "P", "Q", "R",
    "S", "T", "U",
    "V", "W", "X",
    "Y", "Z",
    /*  26  */
};

static const int kCharactersNumber = 36;
static const int kChineseNumber = 0;
static const int kCharsTotalNumber = 36;

static const char* kDefaultSvmPath = "model/svm_hist.xml";
static const char* kLBPSvmPath = "model/svm_lbp.xml";
static const char* kHistSvmPath = "model/svm_hist.xml";

static const char* kDefaultAnnPath = "model/ann.xml";
static const char* kChineseAnnPath = "model/ann_chinese.xml";
static const char* kGrayAnnPath = "model/annCh.xml";

//This is important to for key transform to chinese
static const char* kChineseMappingPath = "model/province_mapping";

static const float kSvmPercentage   = 0.7f;

static const int   kNeurons       = 40;

static const int kGrayCharWidth = 20;
static const int kGrayCharHeight = 32;
static const int kCharLBPGridX = 4;
static const int kCharLBPGridY = 4;
static const int kCharLBPPatterns = 16;

static const int kCharHiddenNeurans = 64;

static const int kCharsCountInOnePlate = 7;
static const int kSymbolsCountInChinesePlate = 6;

static const float kPlateMaxSymbolCount = 7.5f;
static const int kSymbolIndex = 2;

static bool kDebug = false;

#define CV_VERSION_THREE_THREE

// Disable the copy and assignment operator for this class.
#define DISABLE_ASSIGN_AND_COPY(className) \
private:\
    className& operator=(const className&); \
    className(const className&)

// Display the image.
#define SET_DEBUG(param) \
    kDebug = param

// Display the image.
#define SHOW_IMAGE(imgName, debug) \
    if (debug) { \
    namedWindow("imgName", WINDOW_AUTOSIZE); \
    moveWindow("imgName", 500, 500); \
    imshow("imgName", imgName); \
    waitKey(0); \
    destroyWindow("imgName"); \
    }

// Load model. compatitable withe 3.0, 3.1, 3.2 and 3.3
#ifdef CV_VERSION_THREE_THREE
    #define LOAD_SVM_MODEL(model, path) \
    model = ml::SVM::load(path);
    #define LOAD_ANN_MODEL(model, path) \
    model = ml::ANN_MLP::load(path);
#else
    #define LOAD_SVM_MODEL(model, path) \
    model = ml::SVM::load<ml::SVM>(path);
    #define LOAD_ANN_MODEL(model, path) \
    model = ml::ANN_MLP::load<ml::ANN_MLP>(path);
#endif


#endif // GLOBAL_H

