#ifndef NUMBERRECOGNITIONFACTORY_H
#define NUMBERRECOGNITIONFACTORY_H

class QObject;
class QString;
class NumberRecognition;
class Workbench;

class NumberRecognitionFactory
{
public:
    static NumberRecognition *create(const QString &model, Workbench *parent = 0);
};

#endif // NUMBERRECOGNITIONFACTORY_H
