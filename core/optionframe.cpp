/**
* @file optionframe.cpp
* @brief 系统配置框架类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#include <QTabWidget>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QHBoxLayout>

#include "optionframe.h"
#include "optionwidget.h"
#include "workbench.h"
//#include "librarypage.h"
#include "global.h"
#include "profilepage.h"

#include "customdefinepagewidget.h"
#include "systemsetuppagewidget.h"
#include "systemoperationwidget.h"
#include "networkconfigwidget.h"
#include "generalparamwidget.h"
#include "instrumentParamwidget.h"
#include "instrument.h"
#include "numberrecognition.h"

OptionFrame::OptionFrame(Workbench *wb, QWidget *parent)
	: MainWindowBase(parent)
	, m_workbench(wb)
{
	m_currentPage = -1;

	QWidget *center = new QWidget;
	setCentralWidget(center);
    center->resize(800, 600);

	m_tree = new QTreeWidget;
	m_tree->setHeaderHidden(true);
	m_tree->setMinimumWidth(180);

	m_pages = new QStackedWidget;

	QHBoxLayout *horizontalLayout = new QHBoxLayout;
	horizontalLayout->addWidget(m_tree);
	horizontalLayout->addWidget(m_pages, 1);

	center->setLayout(horizontalLayout);

    QTreeWidgetItem *category = addItem(QString::fromLocal8Bit("系统配置"));
    addItem(new CustomDefinePageWidget(workbench()), category);
    addItem(new SystemSetupPageWidget(workbench()), category);
    addItem(new SystemOperationWidget(workbench()), category);
    addItem(new NetworkConfigWidget(workbench()), category);
    addItem(new ProfilePage(workbench()), category);

    category = addItem(QString::fromLocal8Bit("系统参数"));
    addItem(new GeneralParamWidget(workbench()), category);
    addItem(new InstrumentParamWidget(workbench()), category);

    workbench()->instrument()->addOptionPage(this);
    workbench()->algorithm()->addOptionPage(this);

	connect(m_tree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
		this, SLOT(changePage(QTreeWidgetItem *, QTreeWidgetItem *)));
	connect(m_pages, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));

	m_tree->expandAll();
	m_tree->setCurrentItem(m_tree->topLevelItem(0));
	if (m_currentPage < 0)
		onCurrentChanged(m_pages->currentIndex());
}

OptionFrame::~OptionFrame()
{
	saveCurrentPage();
}

QTreeWidgetItem *OptionFrame::addItem(const QString &name, QTreeWidgetItem *parent)
{
	QTreeWidgetItem *item = new QTreeWidgetItem(parent, QStringList() << name);
	if (!parent)
		m_tree->addTopLevelItem(item);
	return item;
}

QTreeWidgetItem *OptionFrame::addItem(QWidget *page, QTreeWidgetItem *parent)
{
	int index = m_pages->addWidget(page);

	QTreeWidgetItem *item = addItem(page->windowTitle(), parent);
	item->setData(0, Qt::UserRole, index + 1);

	return item;
}

void OptionFrame::changePage(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
	if (!current)
		current = previous;

	if (!current)
		return;

	int index;
	do
	{
		index = current->data(0, Qt::UserRole).value<int>();
		current = current->childCount() ? current->child(0) : 0;
	}
	while(!index && current);

	if (!index)
		return;

	m_pages->setCurrentIndex(index-1);
}

void OptionFrame::onCurrentChanged(int current)
{
	saveCurrentPage();

	m_currentPage = current;
	if (m_currentPage >= 0)
	{
        OptionWidget *page = qobject_cast<OptionWidget*>(m_pages->widget(m_currentPage));
		if (page && !m_pageInited.value(m_currentPage))
		{
			page->initializePage();
			if (m_pageInited.size() <= m_currentPage)
				m_pageInited.resize(m_currentPage+1);
			m_pageInited[m_currentPage] = true;
			page->setModified(false);
		}
	}
}

void OptionFrame::saveCurrentPage()
{
	if (m_currentPage >= 0)
	{
        OptionWidget *page = qobject_cast<OptionWidget*>(m_pages->widget(m_currentPage));
		if (page/* && page->isModified()*/)
		{
			WaitCursor wait;
			page->savePage();
			page->setModified(false);
		}
	}
}
