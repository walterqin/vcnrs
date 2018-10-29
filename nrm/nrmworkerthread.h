#ifndef NRMWORKERTHREAD_H
#define NRMWORKERTHREAD_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>

class NRM;
class Workbench;
class WorkList;
class CameraData;

class NRMWorkerThread : public QObject
{
    Q_OBJECT

public:
    explicit NRMWorkerThread(QObject* parent = NULL);
    ~NRMWorkerThread();

    void setWorkList(WorkList *wk = 0);

    void setOwner(NRM *nrm);
    NRM *owner() const { return m_nrm; }

signals:
    void instrumentStarted();
    void recognizedResult(QStringList &);

public slots:
    void process();
    void fetchCameraData(CameraData *mat);
    void removeCameraData();

    void startThread();
    void pause();
    void stop();

private:
    NRM *m_nrm;
    QMutex m_mutex;
    bool m_resume;
    bool m_abort;
    WorkList *m_worklist;
//    QList<ResultData> m_result;         //分析结果
    QList<CameraData> m_removedData;    //已经处理完的数据
};

#endif // NRMWORKERTHREAD_H
