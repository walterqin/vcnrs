/** @brief 代码来自于easyPR项目 */
#ifndef KV_H
#define KV_H

#include <QObject>
#include <QVariant>

#include "util_global.h"

class UTIL_EXPORT Kv : public QObject
{
    Q_OBJECT

public:
    Kv(QString &filename = QString(), QObject *parent = 0);
    ~Kv();

    void load(const QString &file = QString());
    QVariant get(const QString &key);
    void add(const QString &key, const QVariant &value, bool save = true);
    void remove(const QString &key);
    void clear();

signals:
    void valueChanged(const QString &key, const QVariant &value);
    void keyRemoved(const QString &key);

private:
    struct Data;
    Data *d;

    void initial(const QString &filename);
};

#endif // KV_H
