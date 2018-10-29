#include "nrmplugin.h"
#include "nrm.h"

#define KEY "NRM"

NrmPlugin::NrmPlugin()
{

}

NrmPlugin::~NrmPlugin()
{

}

QStringList NrmPlugin::keys() const
{
    return QStringList() << KEY;
}

NumberRecognition *NrmPlugin::create(const QString &key, Workbench *parent)
{
    if (key == KEY)
        return new NRM(parent);
    return 0;
}
