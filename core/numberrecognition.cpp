#include "numberrecognition.h"
#include "workbench.h"
#include "profile.h"
#include "checkstatusthread.h"
#include <QUuid>
#include "zonedetectinterface.h"
#include "charsrecogniseinterface.h"
#include "numberzone.h"
//#include "corefunc.h"

NumberRecognition::NumberRecognition(Workbench *parent)
    : QObject(parent)
    , QMutex(Recursive)
    , m_workbench(parent)
    , m_worklist(0)
    , m_zoneDetectInterface(0)
    , m_charsRecogniseInterface(0)
{


}

NumberRecognition::~NumberRecognition()
{

}

void NumberRecognition::setZoneDetectInterface(ZoneDetectInterface *interf)
{
    if (m_zoneDetectInterface)
        delete m_zoneDetectInterface;
    m_zoneDetectInterface = interf;
}

void NumberRecognition::setCharsRecogniseInterface(CharsRecogniseInterface *interf)
{
    if (m_charsRecogniseInterface)
        delete m_charsRecogniseInterface;
    m_charsRecogniseInterface = interf;
}

int NumberRecognition::numberRecognize(const Mat &src, QVector<NumberZone> &zoneOut, int img_index)
{
    assert(m_zoneDetectInterface != NULL);
    assert(m_charsRecogniseInterface != NULL);

    float scale = 1.0f;
    Mat img = uniformResize(src, scale);
    QVector<NumberZone> zone;
    int result = m_zoneDetectInterface->zoneDectect(src, zone, img_index);
    if (result == 0)
    {
        size_t num = zone.size();
        for (size_t i = 0; i < num; i++)
        {
            NumberZone item = zone.at(i);
            Mat zoneMat = item.getNumberZoneMat();
            //SHOW_IMAGE(zoneMat, 0);

            // 区域图片旋转
            item.setNumberZoneScale(scale);
            RotatedRect rect = item.getNumberZonePos();
 //           item.setNumberZonePos(scaleBackRRect(rect, 1.0f/scale));

            // 获取区域颜色
//            Color color = item.getNumberZoneColor();
//            if (color == UNKNOWN)
//            {

//            }

            std::string zoneIdentify = "";
            int resultCR = m_charsRecogniseInterface->charsRecognise(item, zoneIdentify);
            if (resultCR == 0)
            {
                item.setNumberZoneStr(QString::fromStdString(zoneIdentify));
                zoneOut.push_back(item);

            }
        }
    }

    return zoneOut.count();
}

void NumberRecognition::setUuid(const QUuid &uuid)
{
    if (m_uuid == uuid)
        return;

    m_uuid = uuid;
    emit uuidChanged();
}

void NumberRecognition::setWorklist(WorkList *list)
{
    if (m_worklist == list)
        return;

//    if (m_worklist)
//        disconnect(m_worklist);

    m_worklist = list;
}

