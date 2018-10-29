#ifndef NRMPLUGIN_H
#define NRMPLUGIN_H

#include "numberrecognizeplugin.h"

class NrmPlugin : public NumberRecognizePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID NumberRecogizeFactoryInterface_iid FILE "nrm.json")

public:
    NrmPlugin();
    ~NrmPlugin();

    QStringList keys() const;
    NumberRecognition *create(const QString &key, Workbench *parent);
};

#endif // NRMPLUGIN_H
