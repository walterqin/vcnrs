#ifndef OPTIONPAGE_H
#define OPTIONPAGE_H

#include "nxcore_global.h"
#include <QWidget>

class Workbench;

class NXCORE_EXPORT OptionPage : public QWidget
{
	Q_OBJECT

public:
	explicit OptionPage(Workbench *wb, QWidget *parent = 0);
	~OptionPage();

	Workbench *workbench() const { return m_workbench; }

	void begin();
	void end();
	bool blocked() const;

	bool isModified() const { return m_modified; }

	virtual void initializePage() {}
	virtual void savePage() {}

public slots:
	void setModified(bool modified = true) { m_modified = modified; }

private:
	Workbench *m_workbench;
	int m_blocked;
	bool m_modified;
};

#endif // OPTIONPAGE_H
