#ifndef SIMULATETHREAD_H
#define SIMULATETHREAD_H

#include <QObject>

class CameraData;
class WorkList;
class Instrument;
class QTimer;

class SimulateThread : public QObject
{
    Q_OBJECT
public:
    explicit SimulateThread(QObject *parent = 0);

    void setOwner(Instrument *instr);
//    Instrument owner() const { return m_instr; }
    
    void setWorkList(WorkList *w);
    WorkList *worklist() const { return m_worklist; }

    void setSimulateParam(QMap<QString, QString> &param);

signals:
    void fetchDataCompletement();

public slots:
    void process();

private:
    Instrument *m_instr;
    QMap<QString, QString> m_param;
    WorkList *m_worklist;
    QStringList m_filename;
    QTimer *m_timer;
};

#endif // SIMULATETHREAD_H
