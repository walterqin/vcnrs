/**
* @file global.cpp
* @brief 全局函数的实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-07
*/

#include <QDebug>
#include <QRegExp>
#include <QtScript/QScriptEngine>
#include <QApplication>
#include <Windows.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDate>

#include "global.h"
#include "profile.h"

/**
* @brief 计算比值，表达式可以为 a：b或者a/b形式
* @param [I] 表达式
* @param [I] 是否合法标志
* @retval 结果
*/
double fraction(const QString &expr, bool *ok)
{
    static QRegExp rx("([+-]?\\d+(?:\\.\\d*)?)\\s*[:/]\\s*([+-]?\\d+(?:\\.\\d*)?)");
    int i = rx.indexIn(expr);

    if (ok)
        *ok = i >= 0;

    double v = 0;
    if (i >= 0)
        v = rx.cap(1).toDouble()/rx.cap(2).toDouble();

    return v;
}

/**
* @brief 执行脚本语句
* @param [I] 脚本串
*/
QVariant eval(const QString &expr)
{
    QScriptEngine engine;
    QVariant var = engine.evaluate(expr).toVariant();
    return var;
}


/**
* @brief 获取最上层活动视窗
*/
QWidget *topLevelWidget()
{
    QWidget *parent = qApp->activeModalWidget();
    if (!parent)
        parent = qApp->activeWindow();
    if (parent && parent->window())
        parent = parent->window();
    return parent;
}

/**
* @brief 提示信息显示窗口
* @param [I] 图标
* @param [I] 信息串
* @param [I] 按键
* @param [I] 缺省按键
*/
static QMessageBox::StandardButton showMessageBox(QMessageBox::Icon icon,
                                                  const QString &text,
                                                  QMessageBox::StandardButtons buttons,
                                                  QMessageBox::StandardButton defaultButton)
{
    WaitCursor arrow(Qt::ArrowCursor);

    QWidget *parent = qApp->activeModalWidget();
    if (!parent)
        parent = qApp->activeWindow();
    if (parent && parent->window())
        parent = parent->window();

    QMessageBox box(icon, qApp->applicationName(), text, buttons, parent);
    if (defaultButton != QMessageBox::NoButton)
        box.setDefaultButton(defaultButton);
    return (QMessageBox::StandardButton)box.exec();
}

/**
* @brief 信息显示窗口
* @param [I] 信息串
* @param [I] 按键
* @param [I] 缺省按键
*/
QMessageBox::StandardButton information(const QString &text,
            QMessageBox::StandardButtons buttons,
            QMessageBox::StandardButton defaultButton)
{
    return showMessageBox(QMessageBox::Information, text, buttons, defaultButton);
}

/**
* @brief 询问信息窗口
* @param [I] 信息串
* @param [I] 按键
* @param [I] 缺省按键
*/
QMessageBox::StandardButton question(const QString &text,
         QMessageBox::StandardButtons buttons,
         QMessageBox::StandardButton defaultButton)
{
    return showMessageBox(QMessageBox::Question, text, buttons, defaultButton);
}

/**
* @brief 警告信息显示窗口
* @param [I] 信息串
* @param [I] 按键
* @param [I] 缺省按键
*/
QMessageBox::StandardButton warning(const QString &text,
        QMessageBox::StandardButtons buttons,
        QMessageBox::StandardButton defaultButton)
{
    return showMessageBox(QMessageBox::Warning, text, buttons, defaultButton);
}

/**
* @brief 危险信息显示窗口
* @param [I] 信息串
* @param [I] 按键
* @param [I] 缺省按键
*/
QMessageBox::StandardButton critical(const QString &text,
         QMessageBox::StandardButtons buttons,
         QMessageBox::StandardButton defaultButton)
{
    return showMessageBox(QMessageBox::Critical, text, buttons, defaultButton);
}

/**
* @brief 数据圆整
* @param [I] 数值
* @param [I] 精度
* @param [I] 格式
* @retval 圆整后的数据
*/
double round(double val, int precision, char format)
{
    return QString::number(val, format, precision).toDouble();
}

/**
* @brief 休眠函数
* @param [I] 时间
*/
void sleep(int ms)
{
    Sleep(ms);
}

/**
* @brief 设置数据记录的generate标志
* @param [I] 记录
* @param [I] 字段
* @param [I] 值
*/
void setGeneratedValue(QSqlRecord &rec, const QString &field, const QVariant &value)
{
    int i = rec.indexOf(field);
    rec.setValue(i, value);
    rec.setGenerated(i, true);
}

/**
* @brief CRC16校验
* @param [I] 数值串
* @param [I] 长度
* @return 结果
*/
unsigned short crc16(const void *buf, int len)
{
    static const unsigned short crc_table[256] = {
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
        0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
        0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
        0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
        0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
        0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
        0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
        0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
        0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
        0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
        0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
        0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
        0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
        0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
        0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
        0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
        0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
        0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
        0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
        0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
        0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
        0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
        0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
        0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
        0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
        0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
        0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
        0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
        0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
        0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
        0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
        0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0,
    };

    unsigned short  crc = 0xFFFF;
    unsigned char i;
    const unsigned char *ptr = (const unsigned char *)buf;
    while(len-- > 0)
    {
        i = crc >> 8;
        crc <<= 8;
        crc ^= crc_table[i ^ *ptr++];
    }

    return crc;
}

/**
* @brief 等待光标类构造函数
* @param [I] 构造函数
*/
WaitCursor::WaitCursor(Qt::CursorShape cursor)
{
    qApp->setOverrideCursor(cursor);
}

/**
* @brief 析构函数
*/
WaitCursor::~WaitCursor()
{
    qApp->restoreOverrideCursor();
}

// uniform resize all the image to same size for the next process
Mat uniformResize(const Mat &result, float& scale)
{
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
        scale = float(img_window.cols) / float(nCols);
        resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);

    } else if (nCols <= img_window.cols && nRows > img_window.rows) {
        scale = float(img_window.rows) / float(nRows);
        resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);

    } else if (nCols > img_window.cols && nRows > img_window.rows) {
        float scale1 = float(img_window.cols) / float(nCols);
        float scale2 = float(img_window.rows) / float(nRows);
        scale = scale1 < scale2 ? scale1 : scale2;
        resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);
    } else {
        result_resize = result;
    }
    return result_resize;
}

// Scale back RotatedRect
RotatedRect scaleBackRRect(const RotatedRect& rr, const float scale_ratio)
{
    float width = rr.size.width * scale_ratio;
    float height = rr.size.height * scale_ratio;
    float x = rr.center.x * scale_ratio;
    float y = rr.center.y * scale_ratio;
    RotatedRect mserRect(Point2f(x, y), Size2f(width, height), rr.angle);
    return mserRect;
}


// Scale to small image (for the purpose of comput mser in large image)
Mat scaleImage(const Mat& image, const Size& maxSize, double& scale_ratio)
{
    Mat ret;

    if (image.cols > maxSize.width || image.rows > maxSize.height) {
        double widthRatio = image.cols / (double) maxSize.width;
        double heightRatio = image.rows / (double) maxSize.height;
        double m_real_to_scaled_ratio = max(widthRatio, heightRatio);

        int newWidth = int(image.cols / m_real_to_scaled_ratio);
        int newHeight = int(image.rows / m_real_to_scaled_ratio);

        cv::resize(image, ret, Size(newWidth, newHeight), 0, 0);
        scale_ratio = m_real_to_scaled_ratio;
    } else {
        ret = image;
        scale_ratio = 1.0;
    }

    return ret;
}

// calc safe rect
bool calcSafeRect(const RotatedRect& roi_rect, const Mat& src, Rect_<float> &safeBoundRect)
{
    Rect_<float> boudRect = roi_rect.boundingRect();

    float tl_x = boudRect.x > 0 ? boudRect.x : 0;
    float tl_y = boudRect.y > 0 ? boudRect.y : 0;

    float br_x = boudRect.x + boudRect.width < src.cols
            ? boudRect.x + boudRect.width - 1
            : src.cols - 1;
    float br_y = boudRect.y + boudRect.height < src.rows
            ? boudRect.y + boudRect.height - 1
            : src.rows - 1;

    float roi_width = br_x - tl_x;
    float roi_height = br_y - tl_y;

    if (roi_width <= 0 || roi_height <= 0) return false;

    //  a new rect not out the range of mat

    safeBoundRect = Rect_<float>(tl_x, tl_y, roi_width, roi_height);

    return true;
}

bool calcSafeRect(const RotatedRect &roi_rect, const int width, const int height, Rect_<float> &safeBoundRect)
{
    Rect_<float> boudRect = roi_rect.boundingRect();

    float tl_x = boudRect.x > 0 ? boudRect.x : 0;
    float tl_y = boudRect.y > 0 ? boudRect.y : 0;

    float br_x = boudRect.x + boudRect.width < width
            ? boudRect.x + boudRect.width - 1
            : width - 1;
    float br_y = boudRect.y + boudRect.height < height
            ? boudRect.y + boudRect.height - 1
            : height - 1;

    float roi_width = br_x - tl_x;
    float roi_height = br_y - tl_y;

    if (roi_width <= 0 || roi_height <= 0) return false;

    //  a new rect not out the range of mat

    safeBoundRect = Rect_<float>(tl_x, tl_y, roi_width, roi_height);

    return true;
}

// uniform resize all the plates to same size for the next process
Mat uniformResizePlates(const Mat &result, float& scale)
{
    const int RESULTWIDTH = kPlateResizeWidth;   // 640 930
    const int RESULTHEIGHT = kPlateResizeHeight;  // 540 710

    Mat img_window;
    img_window.create(RESULTHEIGHT, RESULTWIDTH, CV_8UC3);

    int nRows = result.rows;
    int nCols = result.cols;

    Mat result_resize;
    if (nCols <= img_window.cols && nRows <= img_window.rows) {
        result_resize = result;
    }
    else if (nCols > img_window.cols && nRows <= img_window.rows) {
        scale = float(img_window.cols) / float(nCols);
        resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);

    }
    else if (nCols <= img_window.cols && nRows > img_window.rows) {
        scale = float(img_window.rows) / float(nRows);
        resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);

    }
    else if (nCols > img_window.cols && nRows > img_window.rows) {
        float scale1 = float(img_window.cols) / float(nCols);
        float scale2 = float(img_window.rows) / float(nRows);
        scale = scale1 < scale2 ? scale1 : scale2;
        resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);
    }
    else {
        result_resize = result;
    }
    return result_resize;
}

int getBoderColor(Mat img)
{
    assert(img.channels() == 1);
    assert(img.type() == CV_8UC1);
    int w = img.cols;
    int h = img.rows;

    float sum = 0;
    for (int i = 0; i < h; ++i)
    {
        sum += img.at<unsigned char>(i, 0);
        sum += img.at<unsigned char>(i, w-1);
    }
    for (int j = 0; j < w; ++j)
    {
        sum += img.at<unsigned char>(0, j);
        sum += img.at<unsigned char>(h-1, j);
    }

    float avg = sum / float(w + w + h + h);
    return int(avg);
}

// shift an image
Mat translateImg(Mat img, int offsetx, int offsety, int bk)
{
    Mat dst;
    //cout << test << endl;
    Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
    //cout << trans_mat << endl;
    warpAffine(img, dst, trans_mat, img.size(), 1, 0, Scalar(bk));
    return dst;
}

// rotate an image
Mat rotateImg(Mat source, float angle, int bk)
{
    Point2f src_center(source.cols / 2.0F, source.rows / 2.0F);
    Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
    Mat dst;
    warpAffine(source, dst, rot_mat, source.size(), 1, 0, Scalar(bk));
    return dst;
}

// crop the image
Mat cropImg(Mat src, int x, int y, int shift, int bk)
{
    int width = src.cols;
    int height = src.rows;

    int crop_width = width - shift;
    int crop_height = height - shift;

    int x_shift = shift;
    int y_shift = shift;

    x = x < x_shift ? x : x_shift;
    y = y < y_shift ? y : y_shift;

    Rect rect = Rect(x, y, crop_width, crop_height);

    Mat dst = src(rect);
    resize(dst, dst, Size(width, height));
    return dst;
}

Mat generateSyntheticImage(const Mat& image, int use_swap)
{
    int rd = rand();
    int bkColor = getBoderColor(image);
    Mat result = image.clone();
    if (0 && (rd >> 6 & 1))
    {
        int shift = 2;
        int ran_x = rand() % shift;
        int ran_y = rand() % shift;
        result = cropImg(result, ran_x, ran_y, shift, bkColor);
    }
    if (0 && (rd >> 4 & 1))
    {
        int ran_x = rand() % 2 - 1;
        int ran_y = rand() % 2 - 1;
        result = translateImg(result, ran_x, ran_y, bkColor);
    }
    if (1 && (rd >> 2 & 1))
    {
        float angle = float(rand() % 100) * 0.1f - 5.f;
        result = rotateImg(result, angle, bkColor);
    }

    return result;
}
