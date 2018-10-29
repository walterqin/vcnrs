#include "dalsag3gc11plugin.h"
#include "dalsag3gc11.h"

#define DEVICEKEY "DALSAG3GC11"

DalsaG3GC11Plugin::DalsaG3GC11Plugin()
{

}

DalsaG3GC11Plugin::~DalsaG3GC11Plugin()
{

}

QStringList DalsaG3GC11Plugin::keys() const
{
    return QStringList() << DEVICEKEY;
}

Instrument *DalsaG3GC11Plugin::create(const QString &key, Workbench *parent)
{
    if (key == DEVICEKEY)
        return new DalsaG3GC11(parent);
    return 0;
}
