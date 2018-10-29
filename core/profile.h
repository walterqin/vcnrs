/**
* @file profile.h
* @brief 定义系统中用到的数据库操作，包括配置、数据、质控等
* @ingroup common
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-07
*/
#ifndef PROFILE_H
#define PROFILE_H

#include "global.h"
#include <QObject>
#include <QString>
#include <QVariant>
#include <QColor>

#define X_PROPERTY(type, name, defaultValue)			\
private:												\
	QVariant m_##name;									\
														\
public:													\
	type name(const type &val = defaultValue)			\
	{													\
		if (!m_##name.isValid())						\
			m_##name = value(#name, val);				\
		return m_##name.value<type>();					\
	}													\
	void set_##name(const type &val, bool save = true)\
	{													\
		m_##name = val;									\
		setValue(#name, val, save);						\
	}


#define profile (Profile::instance())


class QSqlDatabase;


class CORE_EXPORT Profile : public QObject
{
	Q_OBJECT

	// 通用信息
    X_PROPERTY(QString, corporation, tr("Corporation Name"))			// 公司名称

	// 系统配置
	X_PROPERTY(bool, is_touch_screen, false)					// 触摸屏

public:
	explicit Profile(QObject *parent = 0);
	Profile(const QString &filename, QObject *parent = 0);
	~Profile();

	QSqlDatabase database() const;
	QSqlDatabase qcDatabase() const;
	QSqlDatabase dataDatabase() const;

	void setValue(const QString &key, const QVariant &value, bool save = true);
	QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    template<class T> T value(const QString &key) { return value(key).value<T>(); }
    // 如果一个键没有对应的值, 用另一个缺省值代替(例如全局值)
    QVariant value2(const QString &key, const QString &defaultKey, const QVariant &defaultValue = QVariant());

	void remove(const QString &key);
	QStringList keys() const;

	static Profile &instance();

signals:
	void valueChanged(const QString &key, const QVariant &value);
	void keyRemoved(const QString &key);

private slots:
	void saveValue(const QString &key, const QVariant &value);
	void removeKey(const QString &key);

private:

	struct Data;
	Data *d;

	void init(const QString &filename);
};

#endif // PROFILE_H
