#include "kv.h"
#include <QDebug>
#include <QHash>
#include <QVariant>
#include <QReadWriteLock>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QStringList>

struct Kv::Data
{
    QString filename;
    QHash<QString, QVariant> values;
    QReadWriteLock lock;
};

Kv::Kv(QString &filename, QObject *parent) : QObject(parent)
{
    initial(filename);
}

Kv::~Kv()
{
    delete d;
}

void Kv::load(const QString &file)
{
    initial(file);
}

QVariant Kv::get(const QString &key)
{
    QReadLocker locker(&d->lock);
    QHash<QString, QVariant>::const_iterator it = d->values.find(key);
    if (it != d->values.end())
        return it.value().isValid() ? it.value() : QVariant();
	return QVariant();
}

void Kv::add(const QString &key, const QVariant &value, bool save)
{
    d->lock.lockForWrite();
    d->values[key] = value;
    d->lock.unlock();

    if (save)
        emit valueChanged(key, value);
}

void Kv::remove(const QString &key)
{
    if (key.isEmpty())
        return;

    {
        QWriteLocker locker(&d->lock);
        d->values.remove(key);
    }
    emit keyRemoved(key);
}

void Kv::clear()
{
    d->lock.lockForWrite();
    d->values.clear();
    d->lock.unlock();
}

void Kv::initial(const QString &filename)
{
    d = new Data;
    d->filename = filename;

    if (!filename.isEmpty())
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream in(&file);
        d->lock.lockForWrite();
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList kv = line.split(" ", QString::SkipEmptyParts);
            d->values[kv.at(0)] = kv.at(1);
        }
        d->lock.unlock();

        file.close();
    }
}
