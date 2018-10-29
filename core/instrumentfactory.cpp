#include "instrumentfactory.h"
#include "instrumentplugin.h"
#include "global.h"
#include "vcapp.h"
#include "instrument.h"
//#include "simmodel.h"
#include <QPluginLoader>

Instrument *InstrumentFactory::create(const QString &model, Workbench *parent)
{
//    if (model == "sim")
//        return new SimModel(parent);

    QString filename = model + ".dll";
    QPluginLoader loader(qApp->applicationDirPath() + "/" + filename);
    QObject *obj = loader.instance();
    if (!obj)
    {
        critical(VcApp::tr("Can't load %1\n%2").arg(filename).arg(loader.errorString()));
        loader.unload();
        return 0;
    }

    InstrumentPlugin *plugin = qobject_cast<InstrumentPlugin*>(obj);
    Instrument *instr = plugin ? plugin->create(model.toUpper(), parent) : 0;
    if (!instr)
    {
        critical(VcApp::tr("Invalid plugin %1\n%2").arg(filename).arg(loader.errorString()));
        loader.unload();
        return 0;
    }

    return instr;
}

