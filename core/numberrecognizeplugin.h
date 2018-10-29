#ifndef NUMBERRECOGNIZEPLUGIN_H
#define NUMBERRECOGNIZEPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QFactoryInterface>

#include "core_global.h"

class NumberRecognition;
class Workbench;

class CORE_EXPORT NumberRecogizeFactoryInterface : public QFactoryInterface
{
public:
    virtual NumberRecognition *create(const QString &key, Workbench *parent) = 0;
};

#define NumberRecogizeFactoryInterface_iid "rw.cnra.NumberRecognizeFactoryInterface"
Q_DECLARE_INTERFACE(NumberRecogizeFactoryInterface, NumberRecogizeFactoryInterface_iid)

class CORE_EXPORT NumberRecognizePlugin : public QObject, public NumberRecogizeFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(NumberRecogizeFactoryInterface)

public:
    explicit NumberRecognizePlugin(QObject *parent = 0);
    virtual ~NumberRecognizePlugin();
    virtual QStringList keys() const = 0;

signals:

public slots:
};

#endif // NUMBERRECOGNIZEPLUGIN_H
