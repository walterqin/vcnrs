#ifndef DEBUGPAGE_H
#define DEBUGPAGE_H

#include <QWidget>

namespace Ui {
class DebugPage;
}

class DalsaG3GC11;

class DebugPage : public QWidget
{
    Q_OBJECT

public:
    explicit DebugPage(DalsaG3GC11 *camera, QWidget *parent = 0);
    ~DebugPage();

    void addPage(QWidget *page);

private:
    Ui::DebugPage *ui;
    DalsaG3GC11 *m_instr;
};

#endif // DEBUGPAGE_H
