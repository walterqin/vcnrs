#include "global.h"
#include "zonedetectinterface.h"
#include "numberrecognition.h"

ZoneDetectInterface::ZoneDetectInterface(NumberRecognition *ma, QObject *parent)
    : QObject(parent)
    , m_owner(ma)
{
    m_type = vc::SOBEL;
    m_showDetect = false;
}

ZoneDetectInterface::~ZoneDetectInterface()
{

}

