#include <QString>
#include <QVector>
#include <QPair>

#include "charsidentify.h"
#include "corefunc.h"
#include "config.h"
#include "character.h"
#include "kv.h"

CharsIdentify* CharsIdentify::instance_ = nullptr;

using namespace feature;
using namespace cv;

CharsIdentify::CharsIdentify(QObject *parent)
    : QObject(parent)
{
    LOAD_ANN_MODEL(ann_, kDefaultAnnPath);
    LOAD_ANN_MODEL(annChinese_, kChineseAnnPath);
    LOAD_ANN_MODEL(annGray_, kGrayAnnPath);

    kv_ = QSharedPointer<Kv>(new Kv);
    kv_->load(kChineseMappingPath);

    extractFeature = getGrayPlusProject;
}

CharsIdentify *CharsIdentify::instance()
{
    if (!instance_) {
        instance_ = new CharsIdentify;
    }
    return instance_;
}

int CharsIdentify::classify(cv::Mat f, float& maxVal, bool isChinses, bool isAlphabet)
{
    int result = 0;

    cv::Mat output(1, kCharsTotalNumber, CV_32FC1);
    ann_->predict(f, output);

    maxVal = -2.f;
    if (!isChinses) {
        if (!isAlphabet) {
            result = 0;
            for (int j = 0; j < kCharactersNumber; j++) {
                float val = output.at<float>(j);
                // std::cout << "j:" << j << "val:" << val << std::endl;
                if (val > maxVal) {
                    maxVal = val;
                    result = j;
                }
            }
        }
        else {
            result = 0;
            // begin with 11th char, which is 'A'
            for (int j = 10; j < kCharactersNumber; j++) {
                float val = output.at<float>(j);
                // std::cout << "j:" << j << "val:" << val << std::endl;
                if (val > maxVal) {
                    maxVal = val;
                    result = j;
                }
            }
        }
    }
    else {
        result = kCharactersNumber;
        for (int j = kCharactersNumber; j < kCharsTotalNumber; j++) {
            float val = output.at<float>(j);
            //std::cout << "j:" << j << "val:" << val << std::endl;
            if (val > maxVal) {
                maxVal = val;
                result = j;
            }
        }
    }
    //std::cout << "maxVal:" << maxVal << std::endl;
    return result;
}

void CharsIdentify::classify(cv::Mat featureRows, std::vector<int> &out_maxIndexs,
              std::vector<float> &out_maxVals, std::vector<bool> isChineseVec)
{
    int rowNum = featureRows.rows;

    cv::Mat output(rowNum, kCharsTotalNumber, CV_32FC1);
    ann_->predict(featureRows, output);

    for (int output_index = 0; output_index < rowNum; output_index++) {
        Mat output_row = output.row(output_index);
        int result = 0;
        float maxVal = -2.f;
        bool isChinses = isChineseVec[output_index];
        if (!isChinses) {
            result = 0;
            for (int j = 0; j < kCharactersNumber; j++) {
                float val = output_row.at<float>(j);
                // std::cout << "j:" << j << "val:" << val << std::endl;
                if (val > maxVal) {
                    maxVal = val;
                    result = j;
                }
            }
        }
        else {
            result = kCharactersNumber;
            for (int j = kCharactersNumber; j < kCharsTotalNumber; j++) {
                float val = output_row.at<float>(j);
                //std::cout << "j:" << j << "val:" << val << std::endl;
                if (val > maxVal) {
                    maxVal = val;
                    result = j;
                }
            }
        }
        out_maxIndexs[output_index] = result;
        out_maxVals[output_index] = maxVal;
    }
}

void CharsIdentify::classify(std::vector<Character>& charVec)
{
    size_t charVecSize = charVec.size();

    if (charVecSize == 0)
        return;

    Mat featureRows;
    for (size_t index = 0; index < charVecSize; index++) {
        Mat charInput = charVec[index].getCharacterMat();
        Mat feature = charFeatures(charInput, kPredictSize);
        featureRows.push_back(feature);
    }

    cv::Mat output(charVecSize, kCharsTotalNumber, CV_32FC1);
    ann_->predict(featureRows, output);

    for (size_t output_index = 0; output_index < charVecSize; output_index++) {
        Character& character = charVec[output_index];
        Mat output_row = output.row(output_index);

        int result = 0;
        float maxVal = -2.f;
        std::string label = "";

        bool isChinses = character.getIsChinese();
        if (!isChinses) {
            result = 0;
            for (int j = 0; j < kCharactersNumber; j++) {
                float val = output_row.at<float>(j);
                //std::cout << "j:" << j << "val:" << val << std::endl;
                if (val > maxVal) {
                    maxVal = val;
                    result = j;
                }
            }
            label = std::make_pair(kChars[result], kChars[result]).second;
        }
        else {
            result = kCharactersNumber;
            for (int j = kCharactersNumber; j < kCharsTotalNumber; j++) {
                float val = output_row.at<float>(j);
                //std::cout << "j:" << j << "val:" << val << std::endl;
                if (val > maxVal) {
                    maxVal = val;
                    result = j;
                }
            }
            const char* key = kChars[result];
            std::string s = key;
			std::string province = kv_->get(QString::fromStdString(s)).toString().toStdString();
            label = std::make_pair(s, province).second;
        }
        /*std::cout << "result:" << result << std::endl;
        std::cout << "maxVal:" << maxVal << std::endl;*/
        character.setCharacterScore(maxVal);
        character.setCharacterStr(QString::fromStdString(label));
    }
}

void CharsIdentify::classifyChinese(std::vector<Character>& charVec)
{
    size_t charVecSize = charVec.size();

    if (charVecSize == 0)
        return;

    Mat featureRows;
    for (size_t index = 0; index < charVecSize; index++) {
        Mat charInput = charVec[index].getCharacterMat();
        Mat feature = charFeatures(charInput, kChineseSize);
        featureRows.push_back(feature);
    }

    cv::Mat output(charVecSize, kChineseNumber, CV_32FC1);
    annChinese_->predict(featureRows, output);

    for (size_t output_index = 0; output_index < charVecSize; output_index++) {
        Character& character = charVec[output_index];
        Mat output_row = output.row(output_index);
        bool isChinese = true;

        float maxVal = -2;
        int result = 0;

        for (int j = 0; j < kChineseNumber; j++) {
            float val = output_row.at<float>(j);
            //std::cout << "j:" << j << "val:" << val << std::endl;
            if (val > maxVal) {
                maxVal = val;
                result = j;
            }
        }

        // no match
        if (-1 == result) {
            result = 0;
            maxVal = 0;
            isChinese = false;
        }

        int index = result + kCharsTotalNumber - kChineseNumber;
        const char* key = kChars[index];
        QString s = QString::fromLocal8Bit(key);
        QString province = kv_->get(s).toString();

        /*std::cout << "result:" << result << std::endl;
      std::cout << "maxVal:" << maxVal << std::endl;*/

        character.setCharacterScore(maxVal);
        character.setCharacterStr(province);
        character.setIsChinese(isChinese);
    }
}

void CharsIdentify::classifyChineseGray(std::vector<Character> &charVec)
{
    size_t charVecSize = charVec.size();
    if (charVecSize == 0)
        return;

    Mat featureRows;
    for (size_t index = 0; index < charVecSize; index++) {
        Mat charInput = charVec[index].getCharacterMat();
        cv::Mat feature;
        extractFeature(charInput, feature);
        featureRows.push_back(feature);
    }

    cv::Mat output(charVecSize, kChineseNumber, CV_32FC1);
    annGray_->predict(featureRows, output);

    for (size_t output_index = 0; output_index < charVecSize; output_index++) {
        Character& character = charVec[output_index];
        Mat output_row = output.row(output_index);
        bool isChinese = true;

        float maxVal = -2;
        int result = 0;

        for (int j = 0; j < kChineseNumber; j++) {
            float val = output_row.at<float>(j);
            //std::cout << "j:" << j << "val:" << val << std::endl;
            if (val > maxVal) {
                maxVal = val;
                result = j;
            }
        }

        // no match
        if (-1 == result) {
            result = 0;
            maxVal = 0;
            isChinese = false;
        }

        auto index = result + kCharsTotalNumber - kChineseNumber;
        const char* key = kChars[index];
        QString s = QString::fromLocal8Bit(key);
//        std::string province = kv_->get(s);


        /*std::cout << "result:" << result << std::endl;
        std::cout << "maxVal:" << maxVal << std::endl;*/

        character.setCharacterScore(maxVal);
        character.setCharacterStr(kv_->get(s).toString());
        character.setIsChinese(isChinese);
    }
}

std::pair<std::string, std::string> CharsIdentify::identify(cv::Mat input, bool isChinese, bool isAlphabet)
{
    cv::Mat feature = charFeatures(input, kPredictSize);
    float maxVal = -2;
    auto index = static_cast<int>(classify(feature, maxVal, isChinese, isAlphabet));
    if (index < kCharactersNumber) {
      return std::make_pair(kChars[index], kChars[index]);
    }
    else {
      const char* key = kChars[index];
      std::string s = key;
      std::string province = kv_->get(QString::fromStdString(s)).toString().toStdString();
      return std::make_pair(s, province);
    }

}

int CharsIdentify::identify(std::vector<Mat> inputs,
                            std::vector<std::pair<std::string, std::string> > &outputs,
                            std::vector<bool> isChineseVec)
{
    Mat featureRows;
    size_t input_size = inputs.size();
    for (size_t i = 0; i < input_size; i++) {
        Mat input = inputs[i];
        cv::Mat feature = charFeatures(input, kPredictSize);
        featureRows.push_back(feature);
    }

    std::vector<int> maxIndexs;
    std::vector<float> maxVals;
    classify(featureRows, maxIndexs, maxVals, isChineseVec);

    for (size_t row_index = 0; row_index < input_size; row_index++) {
        int index = maxIndexs[row_index];
        if (index < kCharactersNumber) {
            outputs[row_index] = std::make_pair(kChars[index], kChars[index]);
        }
        else {
            const char* key = kChars[index];
            std::string s = key;
			std::string province = kv_->get(QString::fromStdString(s)).toString().toStdString();
            outputs[row_index] = std::make_pair(s, province);
        }
    }
    return 0;
}

std::pair<std::string, std::string> CharsIdentify::identifyChinese(cv::Mat input, float& out, bool& isChinese)
{
    cv::Mat feature = charFeatures(input, kChineseSize);
    float maxVal = -2;
    int result = 0;

    cv::Mat output(1, kChineseNumber, CV_32FC1);
    annChinese_->predict(feature, output);

    for (int j = 0; j < kChineseNumber; j++) {
        float val = output.at<float>(j);
        //std::cout << "j:" << j << "val:" << val << std::endl;
        if (val > maxVal) {
            maxVal = val;
            result = j;
        }
    }

    // no match
    if (-1 == result) {
        result = 0;
        maxVal = 0;
        isChinese = false;
    }
    else if (maxVal > 0.9){
        isChinese = true;
    }

    int index = result + kCharsTotalNumber - kChineseNumber;
    const char* key = kChars[index];
    std::string s = key;
    std::string province = kv_->get(QString::fromStdString(s)).toString().toStdString();
	out = maxVal;

    return std::make_pair(s, province);
}

std::pair<std::string, std::string> CharsIdentify::identifyChineseGray(cv::Mat input, float& out, bool& isChinese)
{
    cv::Mat feature;
    extractFeature(input, feature);
    float maxVal = -2;
    int result = 0;
    cv::Mat output(1, kChineseNumber, CV_32FC1);
    annGray_->predict(feature, output);

    for (int j = 0; j < kChineseNumber; j++) {
        float val = output.at<float>(j);
        //std::cout << "j:" << j << "val:" << val << std::endl;
        if (val > maxVal) {
            maxVal = val;
            result = j;
        }
    }
    // no match
    if (-1 == result) {
        result = 0;
        maxVal = 0;
        isChinese = false;
    } else if (maxVal > 0.9){
        isChinese = true;
    }
    int index = result + kCharsTotalNumber - kChineseNumber;
    const char* key = kChars[index];
    std::string s = key;
    std::string province = kv_->get(QString::fromStdString(s)).toString().toStdString();
	out = maxVal;
    return std::make_pair(s, province);
}

bool CharsIdentify::isCharacter(cv::Mat input, std::string &label, float& maxVal, bool isChinese)
{
    cv::Mat feature = charFeatures(input, kPredictSize);
    auto index = static_cast<int>(classify(feature, maxVal, isChinese));

    if (isChinese) {
        //std::cout << "maxVal:" << maxVal << std::endl;
    }

    float chineseMaxThresh = 0.2f;

    if (maxVal >= 0.9 || (isChinese && maxVal >= chineseMaxThresh)) {
        if (index < kCharactersNumber) {
            label = std::make_pair(kChars[index], kChars[index]).second;
        }
        else {
            const char* key = kChars[index];
            std::string s = key;
            std::string province = kv_->get(QString::fromStdString(s)).toString().toStdString();
            label = std::make_pair(s, province).second;
        }
        return true;
    }
    else
        return false;
}

void CharsIdentify::LoadModel(QString path)
{
    if (QString::compare(path, QString(kDefaultAnnPath))){
        if (!ann_->empty())
            ann_->clear();
        LOAD_ANN_MODEL(ann_, path.toStdString());
    }
}

void CharsIdentify::LoadChineseModel(QString path)
{
    if (QString::compare(path, QString(kChineseAnnPath))){
        if (!annChinese_->empty())
            annChinese_->clear();
        LOAD_ANN_MODEL(annChinese_, path.toStdString());
    }
}

void CharsIdentify::LoadGrayChANN(QString path)
{
    if (QString::compare(path, QString(kGrayAnnPath))) {
        if (!annGray_->empty())
            annGray_->clear();
        LOAD_ANN_MODEL(annGray_, path.toStdString());
    }
}

void CharsIdentify::LoadChineseMapping(QString path)
{
    kv_->clear();
    kv_->load(path);
}
