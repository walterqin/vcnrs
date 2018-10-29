/**
* @file optionwidget.cpp
* @brief 系统配置页面基类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/

#include "optionwidget.h"

OptionWidget::OptionWidget(Workbench *wb, QWidget *parent)
	: QWidget(parent)
	, m_workbench(wb)
{
	m_blocked = 0;
	m_modified = false;
}

OptionWidget::~OptionWidget()
{

}

void OptionWidget::begin()
{
	m_blocked++;
}

void OptionWidget::end()
{
	m_blocked--;
}

bool OptionWidget::blocked() const
{
	return m_blocked;
}
