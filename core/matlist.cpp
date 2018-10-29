#include "matlist.h"
#include "cameradata.h"

int MatList::finished() const
{
    int i = 0;
    foreach(const CameraData *cd, *this)
        if (cd->isFinished())
            ++i;
    return i;
}

bool MatList::isFinished() const
{
    foreach(const CameraData *cd, *this)
        if (!cd->isFinished())
            return false;
    return true;
}

