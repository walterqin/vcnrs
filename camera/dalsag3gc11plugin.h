#ifndef DALSAG3GC11PLUGIN_H
#define DALSAGE3C11PLUGIN_H

#include "instrumentplugin.h"
#include "camera_global.h"

class DalsaG3GC11Plugin : public InstrumentPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID InstrumentFactoryInterface_iid FILE "camera.json")

public:
    DalsaG3GC11Plugin();
    ~DalsaG3GC11Plugin();

	QStringList keys() const;
    Instrument *create(const QString &key, Workbench *parent);
};

#endif // DALSAGEGC11PLUGIN_H
