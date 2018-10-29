#ifndef CAMERADATA_H
#define CAMERADATA_H

#include <QObject>
#include <opencv2/core.hpp>
#include "core_global.h"
#include "global.h"

using namespace cv;

class Workbench;
class WorkList;

class CORE_EXPORT CameraData : public QObject
{
    Q_OBJECT
public:
    explicit CameraData(Mat &mat, QObject *parent = 0);
    ~CameraData();

    void setCameraNo(vc::CameraNo no) { m_camera = no; }
    vc::CameraNo cameraNo() const { return m_camera; }

    void setCameraData(Mat &mat) { m_mat = mat; }
    Mat getCameraData() const { return m_mat; }

    void setValid(bool valid = true) { m_valid = valid; }
    bool isValid() const { return m_valid; }

	void setWorklist(WorkList *w = 0);

    bool isFinished() const;
    bool isReady() const;
    void setStatus(vc::AnalysisStatus s);
    void setPriority(vc::AnalysisPriority p) { m_priority = p; }

    void setMessage(const QString &msg) { m_message = msg; }
    QString message() const { return m_message; }

    QStringList getResult() { return m_result; }

signals:
    void statusChanged(vc::AnalysisStatus s);
    void finished();

public slots:

private slots:
    

private:
    vc::AnalysisStatus      m_status;		// 当前状态
    vc::AnalysisPriority    m_priority;     // 当前优先级
    mutable QMutex          m_mutex;
    QString                 m_message;
    WorkList                *m_worklist;
    Mat                     m_mat;
    bool                    m_valid;
    QStringList             m_result;
    vc::CameraNo            m_camera;

    friend class Worklist;
};

#endif // CAMERADATA_H
