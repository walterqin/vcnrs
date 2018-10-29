#ifndef CHARSIDENTIFY_H
#define CHARSIDENTIFY_H

#include <QObject>
#include <QtCore>
#include <QSharedPointer>

#include "config.h"
#include "character.h"
#include "feature.h"
#include "kv.h"
#include "opencv2/opencv.hpp"

class NRM_EXPORT CharsIdentify : public QObject
{
    Q_OBJECT
public:
    static CharsIdentify *instance();

    int classify(cv::Mat f, float& maxVal, bool isChinses = false, bool isAlphabet = false);
    void classify(cv::Mat featureRows, std::vector<int>& out_maxIndexs,
                  std::vector<float>& out_maxVals, std::vector<bool> isChineseVec);
    void classify(std::vector<Character> &charVec);

    void classifyChinese(std::vector<Character>& charVec);
    void classifyChineseGray(std::vector<Character>& charVec);

    std::pair<std::string, std::string> identify(cv::Mat input, bool isChinese = false, bool isAlphabet = false);
    int identify(std::vector<cv::Mat> inputs, std::vector<std::pair<std::string, std::string>>& outputs,
                 std::vector<bool> isChineseVec);

    std::pair<std::string, std::string> identifyChinese(cv::Mat input, float& out, bool& isChinese);
    std::pair<std::string, std::string> identifyChineseGray(cv::Mat input, float& out, bool& isChinese);

    bool isCharacter(cv::Mat input, std::string& label, float& maxVal, bool isChinese = false);

    void LoadModel(QString path);
    void LoadChineseModel(QString path);
    void LoadGrayChANN(QString path);
    void LoadChineseMapping(QString path);

private:
    explicit CharsIdentify(QObject *parent = 0);

    feature::annCallback extractFeature;
    static CharsIdentify* instance_;

    // binary character classifer
    cv::Ptr<cv::ml::ANN_MLP> ann_;

    // binary character classifer, only for chinese
    cv::Ptr<cv::ml::ANN_MLP> annChinese_;

    // gray classifer, only for chinese
    cv::Ptr<cv::ml::ANN_MLP> annGray_;

    // used for chinese mapping
    QSharedPointer<Kv> kv_;

signals:

public slots:
};

#endif // CHARSINDENTIFY_H
