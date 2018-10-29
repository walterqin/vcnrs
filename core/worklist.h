#ifndef WORKLIST_H
#define WORKLIST_H

#include <QObject>
#include <QMutex>
#include <QList>
#include "core_global.h"
#include "matlist.h"
#include "instrument.h"

class NumberRecognition;
class Workbench;
class CameraData;

class CORE_EXPORT WorkList : public QObject
{
    Q_OBJECT
public:
    explicit WorkList(Workbench *workbench, QObject *parent = 0);
    ~WorkList();

    Workbench *workbench() const { return m_workbench; }

    void setNumberRecognitionMachine(NumberRecognition *nrm);
    NumberRecognition *nrm() const { return m_nrm; }

    MatList cameraData() const { return m_matList; }

	void setInstrument(Instrument *instr);
    void setInstrumentes(QList<Instrument *> insList);
    Instrument *instrument(int i) const { return m_instrList.at(i); }
    QList<Instrument *> instrumentList() const { return m_instrList; }
    int instrumentCount() const { return m_instrList.count(); }

    bool isRunning() const { return m_isRunning; }
    void setRunning(bool running);

    int finished();

    QMutex *mutex() const { return &m_mutex; }

signals:
    void dataAboutToBeInserted(CameraData *data);
    void dataInserted(CameraData *data);
    void dataAboutToBeRemoved(CameraData *data);
    void dataRemoved(CameraData *data);

public slots:
    void insertSampleData(CameraData *data);
    void removeSampleData(CameraData *data);

private:
    bool m_isRunning;
    mutable QMutex m_mutex;
    Workbench *m_workbench;
    NumberRecognition *m_nrm;
    QList<Instrument*> m_instrList;
    MatList m_matList;
};

#endif // WORKLIST_H
