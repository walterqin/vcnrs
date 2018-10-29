/**
* @file profile.h
* @brief 系统中用到的数据库操作实现
* @ingroup common
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-07
*/

#include <QDebug>
#include <QHash>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDataStream>
#include <QByteArray>
#include <QReadWriteLock>

#include "global.h"
#include "profile.h"

struct Profile::Data 
{
	QString filename;
	QHash<QString, QVariant> values;
	QReadWriteLock lock;
};


static const char* profileName = "profile";
static const char* qcName = "qc";
static const char* dataName = "data";


Profile &Profile::instance()
{
	static Profile profileInstance;
	return profileInstance;
}


Profile::Profile(QObject *parent)
	: QObject(parent)
{
    init("profile.db");
}

Profile::Profile(const QString &filename, QObject *parent)
	: QObject(parent)
{
	init(filename);
}

Profile::~Profile()
{
	delete d;
}

void Profile::init(const QString &filename)
{
	d = new Data;
    d->filename = QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg(filename);
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", profileName);
	db.setDatabaseName(d->filename);
	if (!db.open())
        warning(QString::fromLocal8Bit("无法加载配置文件 %1\n%2").arg(filename).arg(db.lastError().text()));
	if (db.tables().empty())
        warning(QString::fromLocal8Bit("错误的配置文件 %1").arg(filename));

	// 加载所有数据
	QSqlQuery q(db);
	q.exec("select name, value from profile");
    while(q.next())
		d->values[q.value(0).toString()] = q.value(1);

    QString dataFile = QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg("data.db");
	db = QSqlDatabase::addDatabase("QSQLITE", dataName);
	db.setDatabaseName(dataFile);
	if (!db.open())
        warning(QString::fromLocal8Bit("无法打开数据库文件 %1").arg(dataFile));
	if (db.tables().empty())
        warning(QString::fromLocal8Bit("错误的数据库文件 %1").arg(dataFile));

    QString qcFile = QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg("qc.db");
	db = QSqlDatabase::addDatabase("QSQLITE", qcName);
	db.setDatabaseName(qcFile);
	if (!db.open())
        warning(QString::fromLocal8Bit("无法打开数据库文件 %1").arg(qcFile));
	if (db.tables().empty())
        warning(QString::fromLocal8Bit("错误的数据库文件 %1").arg(qcFile));

	connect(this, SIGNAL(valueChanged(QString,QVariant)), this, SLOT(saveValue(QString,QVariant)));
	connect(this, SIGNAL(keyRemoved(QString)), this, SLOT(removeKey(QString)));
}

QSqlDatabase Profile::database() const
{
	return QSqlDatabase::database(profileName);
}

QSqlDatabase Profile::qcDatabase() const
{
	return QSqlDatabase::database(qcName);
}

QSqlDatabase Profile::dataDatabase() const
{
	return QSqlDatabase::database(dataName);
}

void Profile::setValue(const QString &key, const QVariant &value, bool save)
{
	d->lock.lockForWrite();
	d->values[key] = value;
	d->lock.unlock();

	if (save)
		emit valueChanged(key, value);
}

QVariant Profile::value(const QString &key, const QVariant &defaultValue)
{
	{
		QReadLocker locker(&d->lock);
		QHash<QString, QVariant>::const_iterator it = d->values.find(key);
		if (it != d->values.end())
			return it.value().isValid() ? it.value() : defaultValue;
	}

	// 没找到key, 在hash表里添加一项, 以便在"参数配置"界面中能找到并更改
	{
		QWriteLocker locker(&d->lock);
		d->values[key] = QVariant();
	}

	return defaultValue;
}

QVariant Profile::value2(const QString &key, const QString &defaultKey, const QVariant &defaultValue)
{
    QVariant v = value(key);
    if (v.isValid())
        return v;

	return value(defaultKey, defaultValue);
}

void Profile::remove(const QString &key)
{
	{
		QWriteLocker locker(&d->lock);
		d->values.remove(key);
	}
	emit keyRemoved(key);
}

QStringList Profile::keys() const
{
	QWriteLocker locker(&d->lock);
	return d->values.keys();
}

void Profile::saveValue(const QString &key, const QVariant &value)
{
	QSqlDatabase db = QSqlDatabase::database(profileName);
	QSqlQuery query(db);
	if (value.isValid())
	{
		query.prepare("insert or replace into profile (name, value) values(?,?)");
		query.addBindValue(key);
		query.addBindValue(value);
	}
	else
	{
		query.prepare("delete from profile where name = ?");
		query.addBindValue(key);
	}
	query.exec();
}

void Profile::removeKey(const QString &key)
{
	QSqlDatabase db = QSqlDatabase::database(profileName);
	QSqlQuery query(db);
	query.prepare("delete from profile where name = ?");
	query.addBindValue(key);
	query.exec();
}
