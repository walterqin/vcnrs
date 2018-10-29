#ifndef INSTRUMENTFACTORY_H
#define INSTRUMENTFACTORY_H

class QObject;
class QString;
class Instrument;
class Workbench;

class InstrumentFactory
{
public:
    static Instrument *create(const QString &model, Workbench *parent = 0);
};

#endif // INSTRUMENTFACTORY_H
