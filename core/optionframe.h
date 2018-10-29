/**
* @file optionframe.h
* @brief 系统配置框架类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#ifndef OPTIONFRAME_H
#define OPTIONFRAME_H

#include "core_global.h"
#include "mainwindowbase.h"
#include <QVector>

class QTreeWidget;
class QStackedWidget;
class QTreeWidgetItem;
class Workbench;

class CORE_EXPORT OptionFrame : public MainWindowBase
{
	Q_OBJECT

public:
	explicit OptionFrame(Workbench *workbench, QWidget *parent = 0);
	~OptionFrame();

	Workbench *workbench() const { return m_workbench; }

	QTreeWidgetItem *addItem(const QString &name, QTreeWidgetItem *parent = 0);
	QTreeWidgetItem *addItem(QWidget *page, QTreeWidgetItem *parent = 0);

	QTreeWidget *tree() const { return m_tree; }
	QStackedWidget *stack() const { return m_pages; }

private slots:
	void changePage(QTreeWidgetItem * current, QTreeWidgetItem * previous);
	void onCurrentChanged(int current);

private:
	void saveCurrentPage();

	Workbench		*m_workbench;
	QTreeWidget		*m_tree;
	QStackedWidget	*m_pages;
	int				m_currentPage;
	QVector<bool>	m_pageInited;
};

#endif // OPTIONFRAME_H
