#include "numberrecognitionfactory.h"
#include "numberrecognizeplugin.h"
#include "global.h"
#include "vcapp.h"
#include "numberrecognition.h"
#include <QPluginLoader>

NumberRecognition *NumberRecognitionFactory::create(const QString &model, Workbench *parent)
{
    QString filename = model + ".dll";
    QPluginLoader loader(qApp->applicationDirPath() + "/" + filename);
    QObject *obj = loader.instance();
    if (!obj)
    {
        critical(VcApp::tr("Can't load %1\n%2").arg(filename).arg(loader.errorString()));
        loader.unload();
        return 0;
    }

    NumberRecognizePlugin *plugin = qobject_cast<NumberRecognizePlugin*>(obj);
    NumberRecognition *nr = plugin ? plugin->create(model.toUpper(), parent) : 0;
    if (!nr)
    {
        critical(VcApp::tr("Invalid plugin %1\n%2").arg(filename).arg(loader.errorString()));
        loader.unload();
        return 0;
    }

    return nr;
}
