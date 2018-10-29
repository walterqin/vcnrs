#include "zonejudge.h"
#include "corefunc.h"
#include "config.h"
//#include "params.h"

using namespace corefunc;

ZoneJudge *ZoneJudge::m_instance = nullptr;

ZoneJudge* ZoneJudge::instance()
{
    if (!m_instance)
        m_instance = new ZoneJudge;
    return m_instance;
}

ZoneJudge::ZoneJudge(QObject *parent)
    : QObject(parent)
{
    bool useLBP = false;
    if (useLBP)
    {
        LOAD_SVM_MODEL(m_svm, kLBPSvmPath);
        extractFeature = getLBPFeatures;
    }
    else
    {
        LOAD_SVM_MODEL(m_svm, kHistSvmPath);
        extractFeature = getHistomPlusColoFeatures;
    }
}

ZoneJudge::~ZoneJudge()
{

}

void ZoneJudge::loadModel(std::string path)
{
	if (path != std::string(kDefaultSvmPath)) {
		if (!m_svm->empty())
			m_svm->clear();
		LOAD_SVM_MODEL(m_svm, path);
	}
}

// non-maximum suppression
void NMS(std::vector<NumberZone> &inVec, std::vector<NumberZone> &resultVec, double overlap)
{
    std::sort(inVec.begin(), inVec.end());
    std::vector<NumberZone>::iterator it = inVec.begin();
    for (; it != inVec.end(); ++it) {
        NumberZone plateSrc = *it;
        //std::cout << "plateScore:" << plateSrc.getPlateScore() << std::endl;
        Rect rectSrc = plateSrc.getNumberZonePos().boundingRect();
        std::vector<NumberZone>::iterator itc = it + 1;
        for (; itc != inVec.end();) {
            NumberZone plateComp = *itc;
            Rect rectComp = plateComp.getNumberZonePos().boundingRect();
            float iou = computeIOU(QRect(rectSrc.x, rectSrc.y, rectSrc.width, rectSrc.height),
                                   QRect(rectComp.x, rectComp.y, rectComp.width, rectComp.height));
            if (iou > overlap)
                itc = inVec.erase(itc);
            else
                ++itc;
        }
    }
    resultVec = inVec;
}

int ZoneJudge::zoneJudgeUsingNMS(std::vector<NumberZone> &res, std::vector<NumberZone> &result, int maxPlates)
{
    std::vector<NumberZone> zoneVec;
    int num = res.size();
    bool useCascadeJudge = true;

    for (int j = 0; j < num; j++)
    {
        NumberZone zone = zoneVec[j];
        Mat inMat = zone.getNumberZoneMat();
        int result = zoneSetScore(zone);

        if (result == 0)
        {
            if (zone.getNumberZoneLocateType() == CMSER)
            {
                int w = inMat.cols;
                int h = inMat.rows;
                Mat tmpMat = inMat(Rect_<double>(w * 0.05, h * 0.1, w * 0.9, h * 0.8));
                Mat tmpDes = inMat.clone();
                resize(tmpMat, tmpDes, Size(inMat.size()));
                zone.setNumberZoneMat(tmpDes);
                if (useCascadeJudge)
                {
                    int resultCascade = zoneSetScore(zone);
                    if (zone.getNumberZoneLocateType() != CMSER)
                        zone.setNumberZoneMat(inMat);
                    if (resultCascade == 0)
                        zoneVec.push_back(zone);
                }
                else
                {
                    zoneVec.push_back(zone);
                }
            }
            else
                zoneVec.push_back(zone);
        }
    }

    std::vector<NumberZone> reDupPlateVec;
    double overlap = 0.5;
    // double overlap = CParams::instance()->getParam1f();
    // use NMS to get the result plates
    NMS(zoneVec, reDupPlateVec, overlap);
    // sort the plates due to their scores
    std::sort(reDupPlateVec.begin(), reDupPlateVec.end());
    // output the plate judge plates
    std::vector<NumberZone>::iterator it = reDupPlateVec.begin();
    int count = 0;
    for (; it != reDupPlateVec.end(); ++it)
    {
        result.push_back(*it);
        if (0)
        {
            imshow("plateMat", it->getNumberZoneMat());
            waitKey(0);
            destroyWindow("plateMat");
        }
        count++;
        if (count >= maxPlates)
            break;
    }
    return 0;
}

/** @brief Set the score of number zone, 0 is zone, -1 is not*/
int ZoneJudge::zoneSetScore(NumberZone &zone)
{
    Mat feature;
    extractFeature(zone.getNumberZoneMat(), feature);
	float score = m_svm->predict(feature, noArray(), cv::ml::StatModel::Flags::RAW_OUTPUT);
    zone.setNumberZoneScore(score);
    if (score < 0.5)
        return 0;

    return -1;
}

int ZoneJudge::zoneJudge(const Mat &zoneMat)
{
    NumberZone zone;
    zone.setNumberZoneMat(zoneMat);
    return zoneSetScore(zone);
}

int ZoneJudge::zoneJudge(const std::vector<Mat> &res, std::vector<Mat> &result)
{
    int num = res.size();
    int ret = 0;
    for (int j = 0; j < num; j++)
    {
        Mat inMat = res[j];
        int response = zoneJudge(inMat);
        if (response == 0)
        {
            result.push_back(inMat);
            ret++;
        }
    }
    return ret;
}

int ZoneJudge::zoneJudge(const std::vector<NumberZone> &res, std::vector<NumberZone> &result)
{
    int num = res.size();
    int ret = 0;

    for (int i = 0; i < num; i++)
    {
        NumberZone zone = res[i];
        Mat mat = zone.getNumberZoneMat();
        int response = zoneJudge(mat);

        if (response == 0)
        {
            result.push_back(zone);
            ret++;
        }
        else
        {
            int w = mat.cols;
            int h = mat.rows;
            Mat tmpMat = mat(Rect_<double>(w * 0.05, h * 0.1, w * 0.9, h * 0.8));
            Mat tmpDes = mat.clone();
            resize(tmpMat, tmpDes, Size(mat.size()));

            response = zoneJudge(tmpMat);
            if (response == 0)
            {
                result.push_back(zone);
                ret++;
            }
        }
    }
    return ret;
}
