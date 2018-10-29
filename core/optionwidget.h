/**
* @file optionwidget.h
* @brief 系统配置页面基类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#ifndef OPTIONWIDGET_H
#define OPTIONWIDGET_H

#include "core_global.h"
#include <QWidget>

class Workbench;

class CORE_EXPORT OptionWidget : public QWidget
{
	Q_OBJECT

public:
    explicit OptionWidget(Workbench *wb, QWidget *parent = 0);
	~OptionWidget();

	Workbench *workbench() const { return m_workbench; }

	void begin();
	void end();
	bool blocked() const;

	bool isModified() const { return m_modified; }

	virtual void initializePage() {}
	virtual void savePage() {}

public slots:
	void setModified(bool modified = true) { m_modified = modified; }

    void save() {}
    void exit() {}

private:
	Workbench *m_workbench;
	int m_blocked;
	bool m_modified;
};

#endif // OPTIONWIDGET_H
