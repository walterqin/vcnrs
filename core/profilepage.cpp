#include "profilepage.h"
#include "profile.h"
#include "ui_profilepage.h"
#include <QSortFilterProxyModel>
#include <QAction>
#include <QList>
#include <QSet>
#include <QHeaderView>

////////////////////////////////////////////////////////////////////////////////
// ProfileModel

ProfileModel::ProfileModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	m_keys = profile.keys();
	m_keys.sort();
}

int ProfileModel::rowCount(const QModelIndex &parent) const
{
	return m_keys.count() + 1;
}

QVariant ProfileModel::data(const QModelIndex &index, int role) const
{
	if (role != Qt::DisplayRole && role != Qt::EditRole || !index.isValid())
		return QVariant();

	if (index.row() >= m_keys.size())
		return QVariant();

	QString name = m_keys[index.row()];
	switch(index.column())
	{
	case ColumnName:
		return name;

	case ColumnValue:
		return profile.value(name).toString();

	default:
		return QVariant();
	}
}

bool ProfileModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (role != Qt::EditRole || !index.isValid())
		return false;

	if (index.row() == m_keys.size() && index.column() != ColumnName)
		return false;

	QString strValue = value.toString().trimmed();
	QString oldValue = index.data().toString();
	if (strValue == oldValue)
		return false;

	switch(index.column())
	{
	case ColumnName:
		if (index.row() == m_keys.size() && m_keys.indexOf(strValue) < 0)
		{
			beginInsertRows(QModelIndex(), m_keys.size() + 1, m_keys.size() + 1);
			m_keys.append(strValue);
			endInsertRows();

			emit dataChanged(index, index);
			return true;
		}
		return false;

	case ColumnValue:
		if (index.row() < m_keys.size())
		{
			profile.setValue(m_keys[index.row()], strValue.isEmpty() ? QVariant() : strValue);
			emit dataChanged(index, index);

			return true;
		}
		return false;

	default:
		return false;
	}
}

QVariant ProfileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
		return QAbstractTableModel::headerData(section, orientation, role);

	switch(section)
	{
	case ColumnName:
        return QString::fromLocal8Bit("名称");

	case ColumnValue:
        return QString::fromLocal8Bit("值");

	default:
		return QVariant();
	}
}

Qt::ItemFlags ProfileModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QAbstractTableModel::flags(index);

	if (index.row() == m_keys.size())
	{
		if (index.column() == ColumnName)
			flags |= Qt::ItemIsEditable;
	}
	else
	{
		if (index.column() != ColumnName)
			flags |= Qt::ItemIsEditable;
	}
	return flags;
}

bool ProfileModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row + count - 1);
	for(int i=0; i<count && row < m_keys.size(); ++i)
	{
		profile.remove(m_keys[row]);
		m_keys.removeAt(row);
	}
	endRemoveRows();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// ProfilePage

ProfilePage::ProfilePage(Workbench *wb, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfilePage)
{
	ui->setupUi(this);
	m_model = new ProfileModel(this);

	m_proxy = new QSortFilterProxyModel(this);
	m_proxy->setSourceModel(m_model);
	m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

	ui->view->setModel(m_proxy);
	ui->view->resizeColumnToContents(ProfileModel::ColumnName);
    ui->view->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QAction *actionDelete = new QAction(QString::fromLocal8Bit("删除"), this);
	actionDelete->setShortcut(QKeySequence::Delete);
	connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteRows()));
	ui->view->addAction(actionDelete);

	connect(ui->filter, SIGNAL(textEdited(QString)), m_proxy, SLOT(setFilterRegExp(QString)));
}

ProfilePage::~ProfilePage()
{
	delete ui;
}

void ProfilePage::deleteRows()
{
	QSet<int> set;
	foreach(QModelIndex index, ui->view->selectionModel()->selectedIndexes())
		set.insert(index.row());

	QList<int> rows = set.values();
	qSort(rows);

	for(int i=rows.size()-1; i>=0; --i)
		if (!m_proxy->removeRow(rows[i]))
			break;
}
