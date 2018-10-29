#include "zonedetect.h"
#include "utilities.h"
#include "corefunc.h"
#include "config.h"
#include "nrm.h"
#include "zonejudge.h"

using namespace corefunc;
using namespace cv;
using namespace std;

ZoneDetect::ZoneDetect(NRM *owner, QObject *parent)
    : ZoneDetectInterface(owner, parent)
{
    m_zoneLocate = new ZoneLocate(parent);
    m_maxNumberZone = 10;
    m_type = 0;
    m_showDetect = false;
}

ZoneDetect::~ZoneDetect()
{
    SAFE_RELEASE(m_zoneLocate);
}

int ZoneDetect::zoneDetect(Mat src, std::vector<NumberZone> &resultVec, int type, bool showDetectArea, int img_index)
{
    std::vector<NumberZone> sobel_Plates;
    sobel_Plates.reserve(16);
    std::vector<NumberZone> color_Plates;
    color_Plates.reserve(16);
    std::vector<NumberZone> mser_Plates;
    mser_Plates.reserve(16);
    std::vector<NumberZone> all_result_Plates;
    all_result_Plates.reserve(64);

#pragma omp parallel sections
    {
#pragma omp section
        {
            if (!type || type & PR_DETECT_SOBEL)
            {
                m_zoneLocate->zoneSobelLocate(src, sobel_Plates, img_index);
            }
        }
#pragma omp section
        {
            if (!type || type & PR_DETECT_COLOR)
            {
                m_zoneLocate->zoneColorLocate(src, color_Plates, img_index);
            }
        }
#pragma omp section
        {
            if (!type || type & PR_DETECT_CMSER)
            {
                m_zoneLocate->zoneMserLocate(src, mser_Plates, img_index);
            }
        }
    }
    for (int i = 0; i < sobel_Plates.size(); i++)
    {
        NumberZone plate = sobel_Plates.at(i);
        plate.setNumberZoneLocateType(SOBEL);
        all_result_Plates.push_back(plate);
    }
    for (int i = 0; i < color_Plates.size(); i++)
    {
        NumberZone plate = color_Plates.at(i);
        plate.setNumberZoneLocateType(COLOR);
        all_result_Plates.push_back(plate);
    }
    for (int i = 0; i < mser_Plates.size(); i++)
    {
        NumberZone plate = mser_Plates.at(i);
        plate.setNumberZoneLocateType(CMSER);
        all_result_Plates.push_back(plate);
    }
    // use nms to judge plate
    ZoneJudge::instance()->zoneJudgeUsingNMS(all_result_Plates, resultVec, m_maxNumberZone);

    return 0;
}

int ZoneDetect::zoneDetect(Mat src, std::vector<NumberZone> &resultVec, int img_index)
{
    int result = zoneDetect(src, resultVec, m_type, false, img_index);
    return result;
}

void ZoneDetect::LoadSVM(std::string path)
{
    ZoneJudge::instance()->loadModel(path);
}
