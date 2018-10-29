#include "cameradata.h"
#include "worklist.h"

CameraData::CameraData(Mat &mat, QObject *parent)
    : QObject(parent)
    , m_mat(mat)
    , m_worklist(0)
    , m_status(vc::Ready)
    , m_priority(vc::NormalPriority)
{

}

CameraData::~CameraData()
{
    qDelete(m_worklist);
}

bool CameraData::isFinished() const
{
    return m_status == vc::Finished;
}

bool CameraData::isReady() const
{
    return true;
}

void CameraData::setStatus(vc::AnalysisStatus s)
{
    if (m_status == s)
        return;

    m_status = s;
    emit statusChanged(s);

    if (m_status == vc::Finished)
        emit finished();
}

void CameraData::setWorklist(WorkList *w)
{
    if (w == m_worklist)
        return;
    m_worklist = w;
}
