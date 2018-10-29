#ifndef PARAMPAGE_H
#define PARAMPAGE_H

#include "optionwidget.h"
#include <QList>

namespace Ui {
class ParamPage;
}

class DalsaG3GC11;
class QCheckBox;

class ParamPage : public OptionWidget
{
    Q_OBJECT

public:
    explicit ParamPage(DalsaG3GC11 *camera, QWidget *parent = 0);
    ~ParamPage();

    void initializePage();

private slots:
    void onReadParam();
    void onSaveParam();

private:
    bool readParam();
    void setMessage(const QIcon &icon, const QString &text);

    Ui::ParamPage *ui;
    DalsaG3GC11 *m_instr;

};

#endif // PARAMPAGE_H
