#ifndef DALSAG3GC11_H
#define DALSAG3GC11_H

#include "camera_global.h"
#include "instrument.h"
#include "paramheader.h"

class QThread;
class WorkerThreadDalsaG3GC11;

class CAMERA_EXPORT DalsaG3GC11 : public Instrument
{
    Q_OBJECT

public:
    explicit DalsaG3GC11(Workbench *parent = 0);
    ~DalsaG3GC11();

//    QVariant instrumentInfo(Info info) const;
    QString about() const;

    void connectToHost();
    void disconnectFromHost();
    void waitForConnected();

    // 仪器参数, 每个仪器有一个从InstrParamHeader派生出来的参数实例
    const ParamHeader *param() const { return &m_instrParam; }
	void setParam(const ParamHeader *pa) { m_instrParam = *pa;  }

    // 打开仪器内部用于读写参数的文档
    QIODevice *openParamFile(QIODevice::OpenMode mode);

    void addOptionPage(OptionFrame *frame);
    void initMainWindow(QMainWindow *window);
    void manualTest();

    void updateStatus();

signals:
    void cameraOpened();
    void cameraClosed();

    void acquisitionStart();
    void acquisitionPause();
    void acquisitionStop();

    void acquisedImage();


public slots:
    void onOpenCamera();
    void onCloseCamera();

    void start();
    void pause();
    void end();
    bool reset();
    void clean();

private:
    void imageAcquisitionProcess();

private:
    ParamHeader m_instrParam;
    mutable QMutex m_commMutex;
    WorkerThreadDalsaG3GC11 *m_worker;
    QThread *m_workerThread;
};

#endif // DALSAG3GC11_H
