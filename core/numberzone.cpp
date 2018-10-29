#include <QtCore>
#include "numberzone.h"

NumberZone::NumberZone(QObject *parent)
    : QObject(parent)
{
    m_score = -1;
    m_numberZoneStr = "";
    m_numberZoneColor = UNKNOWN;
}

NumberZone::NumberZone(const NumberZone &other)
{
    m_numberZoneMat = other.m_numberZoneMat;
    m_chineseMat    = other.m_chineseMat;
    m_chineseKey    = other.m_chineseKey;
    m_score         = other.m_score;
    m_numberZonePos = other.m_numberZonePos;
    m_numberZoneStr = other.m_numberZoneStr;
    m_locateType    = other.m_locateType;
    m_numberZoneColor= other.m_numberZoneColor;
    m_line          = other.m_line;
    m_leftPoint     = other.m_leftPoint;
    m_rightPoint    = other.m_rightPoint;
    m_mergeCharRect = other.m_mergeCharRect;
    m_maxCharRect   = other.m_maxCharRect;
    m_scale         = other.m_scale;
    m_distVec       = other.m_distVec;

    m_mserCharVec   = other.m_mserCharVec;
    m_reutCharVec   = other.m_reutCharVec;
    m_ostuLevel     = other.m_ostuLevel;
}

NumberZone& NumberZone::operator=(const NumberZone& other)
{
    if (this != &other)
    {
        m_numberZoneMat	= other.m_numberZoneMat;
        m_chineseMat    = other.m_chineseMat;
        m_chineseKey    = other.m_chineseKey;
        m_score         = other.m_score;
        m_numberZonePos	= other.m_numberZonePos;
        m_numberZoneStr	= other.m_numberZoneStr;
        m_locateType    = other.m_locateType;
        m_numberZoneColor= other.m_numberZoneColor;
        m_line          = other.m_line;
        m_leftPoint     = other.m_leftPoint;
        m_rightPoint    = other.m_rightPoint;
        m_mergeCharRect = other.m_mergeCharRect;
        m_maxCharRect   = other.m_maxCharRect;

        m_distVec       = other.m_distVec;

        m_mserCharVec   = other.m_mserCharVec;
        m_reutCharVec   = other.m_reutCharVec;
        m_ostuLevel     = other.m_ostuLevel;
    }
    return *this;
}
NumberZone::~NumberZone()
{

}

