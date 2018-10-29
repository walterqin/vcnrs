#ifndef CHARSRECOGNISEINTERFACE_H
#define CHARSRECOGNISEINTERFACE_H

#include <QObject>
#include "core_global.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

using namespace cv;
class Workbench;
class NumberZone;
class NumberRecognition;

class CORE_EXPORT CharsRecogniseInterface : public QObject
{
    Q_OBJECT
public:
    CharsRecogniseInterface(NumberRecognition *ma, QObject *parent = 0);
    ~CharsRecogniseInterface();

    NumberRecognition *owner() const { return m_owner; }

	virtual int charsRecognise(cv::Mat src, std::string &plateLicense) { return 0; }
	virtual int charsRecognise(NumberZone &zone, std::string &plateLicense) { return 0; }

private:
    NumberRecognition *m_owner;
};

#endif // CHARSRECOGNISEINTERFACE_H
