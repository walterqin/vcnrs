/**
* @file customdefinepagewidget.h
* @brief 用户自定义设置类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#ifndef CUSTOMDEFINEPAGEWIDGET_H
#define CUSTOMDEFINEPAGEWIDGET_H

#include <QWidget>
#include <QStyledItemDelegate>
#include "optionwidget.h"

class QSqlTableModel;
class QTableView;
class QSqlQuery;
class QStyleOptionViewItem;
class QPushButton;

/////////////////////////////////////////////////////////////////////////
/// \brief The ItemDelegate class
///
class ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    enum {ItemName, DataType, DataLen, DispName, DispOrder, IsEdit, Remark};

    explicit ItemDelegate(QObject *parent=0)
        : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
};


//////////////////////////////////////////////////////////////////////
/// \brief The CustomDefinePageWidget class
///
class CustomDefinePageWidget : public OptionWidget
{
    Q_OBJECT

public:
    CustomDefinePageWidget(Workbench *wb, QWidget *parent = 0);
    ~CustomDefinePageWidget();

    virtual void initializePage();
    virtual void savePage();

private slots:
    void save();
    void exit();

private:
    QSqlTableModel *m_customModel; /**< 自定义项参数 */
    QTableView *m_customView;
    QSqlQuery *m_customQuery;
    QPushButton *m_saveBtn;
};

#endif // CUSTOMDEFINEPAGEWIDGET_H
