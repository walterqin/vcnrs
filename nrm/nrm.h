#ifndef NRM_H
#define NRM_H

#include "nrm_global.h"
#include "numberrecognition.h"
#include "global.h"
#include "config.h"

class Workbench;
class OptionFrame;
class ZoneDetect;
class ZoneJudge;
class ZoneLocate;
class CharsIdentify;
class CharsRecognise;
class QThread;
class NRMWorkerThread;

class NRM_EXPORT NRM : public NumberRecognition
{
    Q_OBJECT

public:
    explicit NRM(Workbench *parent = 0);
    ~NRM();

    Workbench *workbench() const { return m_workbench; }

    void setSimulate(bool sim);
    bool isSimulate() const { return m_sim; }

    void addOptionPage(OptionFrame *frame);
    void setDebug(bool flag = false);
    void manualTest();

    int numberRecognize(const Mat &src, QVector<NumberZone> &zoneOut, int img_index);
    int numberRecognize(const Mat &src, QVector<NumberZone> &zoneOut);

    void setLifeMode(bool mode);
    void setDetectType(int type);

    void loadSVM(QString path);
    void loadANN(QString path);
    void loadChineseANN(QString path);
    void loadGrayChANN(QString path);
    void loadChineseMapping(QString path);

public slots:
    void onStartAnalysis();
    void onStopAnalysis();

signals:
    void startWorkerThread();
    void stopWorkerThread();

private:
    void startAnalyisThread();

private:
    bool m_sim;
    NRMWorkerThread *m_worker;
    Workbench *m_workbench;
    ZoneDetect *m_zoneDetect;
    ZoneJudge *m_zoneJudge;
    ZoneLocate *m_zoneLocate;
    CharsIdentify *m_charsIdentify;
    CharsRecognise *m_charsRecognise;
    QThread *m_workerThread;

private:
    DISABLE_ASSIGN_AND_COPY(NRM);
};

#endif // NRM_H
