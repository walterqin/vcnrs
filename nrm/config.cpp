#include "config.h"
#include <QVector>

Config::Config()
{
    m_param1f.clear();
    m_param2f.clear();
    m_param3f.clear();

    m_param1i.clear();
    m_param2i.clear();
    m_param3i.clear();

    m_param1b.clear();
    m_param2b.clear();
    m_param3b.clear();
}

Config::Config(const Config& other)
{
    m_param1f = other.getParam1f();
    m_param2f = other.getParam2f();
    m_param3f = other.getParam3f();

    m_param1i = other.getParam1i();
    m_param2i = other.getParam2i();
    m_param3i = other.getParam3i();

    m_param1b = other.getParam1b();
    m_param2b = other.getParam2b();
    m_param3b = other.getParam3b();
}

void Config::setParam1f(float* param, int size)
{
    for (int i = 0; i < size; i++)
        m_param1f.append(*(param + i));
}


void Config::setParam2f(float* param, int size)
{
    for (int i = 0; i < size; i++)
        m_param2f.append(*(param + i));
}

void Config::setParam3f(float* param, int size)
{
    for (int i = 0; i < size; i++)
        m_param3f.append(*(param + i));
}

void Config::setParam1i(int* param, int size)
{
    for (int i = 0; i < size; i++)
        m_param1i.append(*(param + i));
}


void Config::setParam2i(int* param, int size)
{
    for (int i = 0; i < size; i++)
        m_param2i.append(*(param + i));
}

void Config::setParam3i(int* param, int size)
{
    for (int i = 0; i < size; i++)
        m_param3i.append(*(param + i));
}

void Config::setParam1b(bool* param, int size)
{
    for (int i = 0; i < size; i++)
        m_param1b.append(*(param + i));
}

void Config::setParam2b(bool* param, int size)
{
    for (int i = 0; i < size; i++)
        m_param2b.append(*(param + i));
}

void Config::setParam3b(bool* param, int size)
{
    for (int i = 0; i < size; i++)
        m_param3b.append(*(param + i));
}
