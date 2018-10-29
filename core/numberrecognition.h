#ifndef NUMBERRECOGNITION_H
#define NUMBERRECOGNITION_H

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QReadWriteLock>
#include <QUuid>

#include "core_global.h"
//#include "zonedetectinterface.h"
//#include "charsrecogniseinterface.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

class Workbench;
class NumberZone;
class Character;
class OptionFrame;
class WorkList;
class QMainWindow;
class ZoneDetectInterface;
class CharsRecogniseInterface;

using namespace cv;

class CORE_EXPORT NumberRecognition : public QObject, public QMutex
{
    Q_OBJECT

    enum InforCode
    {
        InfoOK = 0,
        InfoUnrecognizable,
        InfoUnknown,
    };

public:
    explicit NumberRecognition(Workbench *parent = 0);
    virtual ~NumberRecognition();

	Workbench *workbench() const { return m_workbench; }

    void setZoneDetectInterface(ZoneDetectInterface *interf);
    ZoneDetectInterface *zoneDetectInterface() const { return m_zoneDetectInterface; }
    void setCharsRecogniseInterface(CharsRecogniseInterface *interf);
    CharsRecogniseInterface *charsRecogniseInterface() const { return m_charsRecogniseInterface; }

    void setWorklist(WorkList *list);
    WorkList *worklist() const { return m_worklist; }

    virtual int numberRecognize(const Mat &src, QVector<NumberZone> &zoneOut, int img_index = 0);
    virtual void addOptionPage(OptionFrame *frame) {}

    void setResultShow(bool isShow = false) { m_showResult = isShow; }
    bool getResultShow() const { return m_showResult; }
    QUuid uuid() const { return m_uuid; }
    void setUuid(const QUuid &uuid);

public slots:

signals:
    void uuidChanged();
    void charsRecogised(QVector<NumberZone> &chars);
    void fetalError(int code, const QString &detail);

private:
    ZoneDetectInterface *m_zoneDetectInterface;
    CharsRecogniseInterface *m_charsRecogniseInterface;

private:
    QUuid m_uuid;
    Workbench *m_workbench;
    bool m_showResult;
    WorkList *m_worklist;
};

#endif // NUMBERRECOGNITION_H
