#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QWidget>
#include <QAbstractTableModel>

namespace Ui {
class ProfilePage;
}

class QSortFilterProxyModel;
class Workbench;

class ProfileModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum { ColumnName, ColumnValue, ColumnCount };

	explicit ProfileModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const { return ColumnCount; }

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	QVariant headerData(int section, Qt::Orientation orientation,
						int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;

	bool removeRows(int row, int count, const QModelIndex &parent);

private:
	QStringList m_keys;
};

class ProfilePage : public QWidget
{
	Q_OBJECT
	
public:
	explicit ProfilePage(Workbench *wb, QWidget *parent = 0);
	~ProfilePage();

private slots:
	void deleteRows();
	
private:
	Ui::ProfilePage *ui;
	ProfileModel *m_model;
	QSortFilterProxyModel *m_proxy;
};

#endif // PROFILEPAGE_H
