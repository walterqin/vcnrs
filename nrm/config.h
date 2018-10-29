#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore>
#include <QMessageBox>
#include "nrm_global.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "global.h"


class NRM_EXPORT  Config {
public:
    Config();
    Config(const Config& other);

    void setParam1f(float* param, int size);
    QVector<float> getParam1f() const  { return m_param1f; }

    void setParam2f(float* param, int size);
    QVector<float> getParam2f() const { return m_param2f; }

    void setParam3f(float* param, int size);
    QVector<float> getParam3f() const { return m_param3f; }

    void setParam1i(int* param, int size);
    QVector<int> getParam1i() const { return m_param1i; }

    void setParam2i(int* param, int size);
    QVector<int> getParam2i() const { return m_param2i; }

    void setParam3i(int* param, int size);
    QVector<int> getParam3i() const { return m_param3i; }

    void setParam1b(bool* param, int size);
    QVector<bool> getParam1b() const { return m_param1b; }

    void setParam2b(bool* param, int size);
    QVector<bool> getParam2b() const { return m_param2b; }

    void setParam3b(bool* param, int size);
    QVector<bool> getParam3b() const { return m_param3b; }

private:
    //! float params
    QVector<float> m_param1f;
    QVector<float> m_param2f;
    QVector<float> m_param3f;

    //! int params
    QVector<int> m_param1i;
    QVector<int> m_param2i;
    QVector<int> m_param3i;

    //! bool params
    QVector<bool> m_param1b;
    QVector<bool> m_param2b;
    QVector<bool> m_param3b;
};


#endif // CONFIG_H

