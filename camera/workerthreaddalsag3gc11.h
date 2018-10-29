#ifndef WORKERTHREADDALSAG3GC11_H
#define WORKERTHREADDALSAG3GC11_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QList>

class Workbench;
class DalsaG3GC11;
class Instrument;
class WorkList;
class CameraData;

class WorkerThreadDalsaG3GC11 : public QObject
{
    Q_OBJECT
public:
    WorkerThreadDalsaG3GC11(QObject *parent = 0);
    ~WorkerThreadDalsaG3GC11();

    void setOwner(Instrument *instr);
    Instrument *owner() const { return reinterpret_cast<Instrument *>(m_instrument); }

    bool isRunning();

signals:
    void dataSampledCompletement();

public slots:
    void start();
    void pause();
    void stop();

    void process();

private:
    DalsaG3GC11 *m_instrument;
    QMutex m_mutex;
    QWaitCondition m_condition;
    bool m_resume;
    bool m_stop;
    bool m_running;
};

#endif // WORKERTHREADDALSAG3GC11_H
