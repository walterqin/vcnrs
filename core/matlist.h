#ifndef MATLIST_H
#define MATLIST_H

#include <QList>
#include "core_global.h"
#include "cameradata.h"

class CORE_EXPORT MatList : public QList<CameraData*>
{
public:
    MatList() {}
    ~MatList() {}

    int finished() const;
    bool isFinished() const;
};

#endif // MATLIST_H
