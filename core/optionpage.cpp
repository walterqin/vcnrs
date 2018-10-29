#include "optionpage.h"

OptionPage::OptionPage(Workbench *wb, QWidget *parent)
	: QWidget(parent)
	, m_workbench(wb)
{
	m_blocked = 0;
	m_modified = false;
}

OptionPage::~OptionPage()
{

}

void OptionPage::begin()
{
	m_blocked++;
}

void OptionPage::end()
{
	m_blocked--;
}

bool OptionPage::blocked() const
{
	return m_blocked;
}
