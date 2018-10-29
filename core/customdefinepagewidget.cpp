/**
* @file customdefinepagewidget.cpp
* @brief 用户自定义设置类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#include <QtGui>
#include <QtCore>
#include <QWidget>
#include <QComboBox>
#include <QSqlQuery>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QStyleOptionViewItem>
#include <QTableView>
#include <QHeaderView>

#include "customdefinepagewidget.h"
#include "profile.h"

//////////////////////////////////////////////////////////////////
/// \brief The ItemDelegate class
/// \brief
void ItemDelegate::paint(QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}

QWidget *ItemDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    static QStringList dataType;
    static QStringList isEdit;
    if (dataType.isEmpty())
        dataType << QString::fromLocal8Bit("字符型") << QString::fromLocal8Bit("数值型") << QString::fromLocal8Bit("布尔型");
    if (isEdit.isEmpty())
        isEdit << QString::fromLocal8Bit("是") << QString::fromLocal8Bit("否");

    if (index.column() == DataType)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems(dataType);
        return editor;
    }

    if (index.column() == IsEdit)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems(isEdit);
        return editor;
    }

    if (index.column() == Remark | index.column() == DataLen |
            index.column() == DispName)
        return new QLineEdit(parent);

    return 0;
}

void ItemDelegate::setEditorData(QWidget *editor,
        const QModelIndex &index) const
{
    if (index.column() == DataType | index.column() == IsEdit)
    {
        QString value = index.model()->data(index).toString();
        QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
        Q_ASSERT(comboBox);
        comboBox->setCurrentIndex(comboBox->findText(value));
    }
    else if (index.column() == Remark | index.column() == DataLen |
             index.column() == DispName)
    {
        QString value = index.model()->data(index).toString();
        QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
        Q_ASSERT(lineEdit);
        lineEdit->setText(value);
    }
    else
        QStyledItemDelegate::setEditorData(editor, index);
}

void ItemDelegate::setModelData(QWidget *editor,
        QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == DataType | index.column() == IsEdit)
    {
        QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
        Q_ASSERT(comboBox);
        model->setData(index, comboBox->currentText());
    }
    else if (index.column() == Remark | index.column() == DataLen |
             index.column() == DispName)
    {
        QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
        Q_ASSERT(lineEdit);
        model->setData(index, lineEdit->text());
    }
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}


/////////////////////////////////////////////////////////////////////////
/// \brief CustomDefinePageWidget类
///
CustomDefinePageWidget::CustomDefinePageWidget(Workbench *wb, QWidget *parent)
    : OptionWidget(wb, parent)
{
    this->setWindowTitle(QString::fromLocal8Bit("自定义表项"));
    setWindowModality(Qt::NonModal);

    initializePage();
    connect(m_saveBtn, SIGNAL(clicked()), this, SLOT(save()));
}

CustomDefinePageWidget::~CustomDefinePageWidget()
{

}

void CustomDefinePageWidget::initializePage()
{
    m_saveBtn = new QPushButton(QString::fromLocal8Bit("保存"));

    QSqlDatabase db = profile.database();
    m_customModel = new QSqlTableModel(this, db);
    m_customModel->setTable("customItem");
    m_customModel->select();

    m_customView = new QTableView;
    m_customView->setModel(m_customModel);
    m_customView->setItemDelegate(new ItemDelegate(this));
    m_customView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_customView->resizeColumnsToContents();
    m_customView->setColumnWidth(0, 100);
    m_customView->setColumnWidth(3, 150);
    m_customView->horizontalHeader()->setStretchLastSection(true);

    QHBoxLayout *funcLayout = new QHBoxLayout;
    funcLayout->addStretch();
    funcLayout->addWidget(m_saveBtn);
    funcLayout->addSpacing(30);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_customView);
    mainLayout->addLayout(funcLayout);

//    setLayout(mainLayout);
}

void CustomDefinePageWidget::savePage()
{

}

void CustomDefinePageWidget::save()
{

}

void CustomDefinePageWidget::exit()
{

}
