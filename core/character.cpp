#include <QRect>
#include <QPoint>
#include "character.h"

Character::Character(QObject *parent)
    : QObject(parent)
{
    m_characterMat = Mat();
    m_characterGrayMat = Mat();
    m_characterPos = QRect(0, 0, 0, 0);
    m_characterStr = "";
    m_score = 0;
    m_isChinese = false;
    m_ostuLevel = 125;
    m_center = QPoint(0, 0);
    m_index = 0;
}

Character::Character(const Character& other)
{
    m_characterMat = other.m_characterMat;
    m_characterGrayMat = other.m_characterGrayMat;
    m_characterPos = other.m_characterPos;
    m_characterStr = other.m_characterStr;
    m_score = other.m_score;
    m_isChinese = other.m_isChinese;
    m_ostuLevel = other.m_ostuLevel;
    m_center = other.m_center;
    m_index = other.m_index;
}

Character::~Character()
{

}
Character& Character::operator =(const Character &other)
{
    if (this != &other)
    {
        m_characterMat = other.m_characterMat;
        m_characterGrayMat = other.m_characterGrayMat;
        m_characterPos = other.m_characterPos;
        m_characterStr = other.m_characterStr;
        m_score = other.m_score;
        m_isChinese = other.m_isChinese;
        m_ostuLevel = other.m_ostuLevel;
        m_center = other.m_center;
        m_index = other.m_index;
    }
    return *this;
}
