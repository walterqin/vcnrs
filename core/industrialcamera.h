#ifndef INDUSTRIALCAMERA_H
#define INDUSTRIALCAMERA_H

#include <QObject>
#include <QIODevice>

#include "core_global.h"

class CORE_EXPORT IndustrialCamera : public QIODevice
{
public:
    IndustrialCamera(QObject *parent = 0);
};

#endif // INDUSTRIALCAMERA_H
