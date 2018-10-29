#ifndef TRAINMODELPAGE_H
#define TRAINMODELPAGE_H

#include "optionwidget.h"

namespace Ui {
class TrainModelPage;
}

class NRM;

class TrainModelPage : public OptionWidget
{
    Q_OBJECT

public:
    TrainModelPage(NRM *nrm, QWidget *parent = 0);
    ~TrainModelPage();

    void initializePage();

public slots:
    void onTrainSvm();
    void onTrainAnn();
    void onTrainAnnZh();

    void onSvmSelDataTrainPath();
    void onSvmSelTestTrainPath();
    void onSvmSelUndataTrainPath();
    void onSvmSelUnTestTrainPath();
    void onSvmSetModelFileName();

    void onAnn1SelDataTrainPath();
    void onAnn1SelTestTrainPath();
    void onAnn1SelUndataTrainPath();
    void onAnn1SelUnTestTrainPath();
    void onAnn1SetModelFileName();

    void onAnn2SelDataTrainPath();
    void onAnn2SelTestTrainPath();
    void onAnn2SelUndataTrainPath();
    void onAnn2SelUnTestTrainPath();
    void onAnn2SetModelFileName();

private:
    Ui::TrainModelPage *ui;
    NRM *m_nrm;

    QString m_svmDataPath;
    QString m_svmTestPath;
    QString m_svmUndataPath;
    QString m_svmUnTestPath;
    QString m_svmModelPath;

    QString m_ann1DataPath;
    QString m_ann1TestPath;
    QString m_ann1UndataPath;
    QString m_ann1UnTestPath;
    QString m_ann1ModelPath;

    QString m_ann2DataPath;
    QString m_ann2TestPath;
    QString m_ann2UndataPath;
    QString m_ann2UnTestPath;
    QString m_ann2ModelPath;
};

#endif // TRAINMODELPAGE_H
