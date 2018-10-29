#include <QEvent>
#include <QStyle>
#include <QApplication>
#include <QMainWindow>
#include <QStatusBar>
#include <QDesktopWidget>
#include <QTimer>
#include <QScrollBar>

#include "messagewidget.h"
#include "ui_messagewidget.h"
#include "vcapp.h"
#include "workbench.h"

///////////////////////////////////////////////////////////////////////////////
// MessageModel

MessageModel::MessageModel(QObject *parent) : QAbstractListModel(parent)
{
    m_count.resize(4);
    m_confirmIndex = 0;
}

int MessageModel::rowCount(const QModelIndex &parent) const
{
    return m_messages.size();
}

int MessageModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Message &msg = m_messages[index.row()];
    switch(role)
    {
    case Qt::EditRole:
    case Qt::DisplayRole:
        return msg.fullText;
    case Qt::DecorationRole:
        return qApp->style()->standardIcon(
                    msg.type == QtDebugMsg   ? QStyle::SP_MessageBoxInformation :
                    msg.type == QtWarningMsg ? QStyle::SP_MessageBoxWarning :
                                               QStyle::SP_MessageBoxCritical);
    case Qt::ForegroundRole:
        return QColor(msg.confirmed || msg.restored ? Qt::darkGray : Qt::black);
    default:
        return QVariant();
    }
}

// 添加一条消息, 添加成功返回true, 如果消息被忽略(比如重复的消息)返回false
bool MessageModel::showMessage(Workbench *wb, int channel, int type, const QString &text, bool show)
{
    QTime now = QTime::currentTime();
    QList<int> indices = m_msgIndex.values(text);

    if (show)
    {
        foreach(int i, indices)
        {
            Message &msg = m_messages[i];
            if (msg.workbench == wb && msg.channel == channel && msg.type == type)
                return false;
        }

        beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));

        Message msg;
        msg.workbench = wb;
        msg.type = type;
        msg.channel = channel;
        msg.text = text;
        msg.confirmed = false;
        msg.restored = false;
        msg.time = now;
        msg.fullText = QString("%1 %2")
                .arg(msg.time.toString())
                .arg(msg.text);
        m_msgIndex.insert(msg.text, rowCount(QModelIndex()));
        m_messages.append(msg);

        if ((unsigned)type < m_count.size())
            ++m_count[type];

        endInsertRows();

        return true;
    }
    else
    {
        bool modified = false;
        foreach(int i, indices)
        {
            Message &msg = m_messages[i];
            if (msg.channel != channel || msg.type != type)
                continue;

            // 报警恢复了
            if ((unsigned)type < m_count.size() && m_count[type] > 0)
                --m_count[type];

            msg.restored = true;
            msg.restoreTime = QTime::currentTime();
            m_msgIndex.remove(msg.text, i);
            modified = true;

            emit dataChanged(index(i, 0), index(i, columnCount(QModelIndex()) - 1));
        }

        return modified;
    }
}

int MessageModel::messageCount(int type) const
{
    return m_count.value(type);
}

bool MessageModel::hasWarnings() const
{
    return  m_count[QtCriticalMsg] > 0 ||
            m_count[QtFatalMsg] > 0;
}

void MessageModel::confirm()
{
    for(int i=m_confirmIndex; i<m_messages.size(); ++i)
    {
        if (!m_messages[i].confirmed &&
                !m_messages[i].restored &&
                m_messages[i].type > QtWarningMsg)
        {
            m_messages[i].confirmed = true;
            if (m_count.value(m_messages[i].type) > 0)
                --m_count[m_messages[i].type];

            emit dataChanged(index(i, 0), index(i, columnCount(QModelIndex()) - 1));
        }
    }
    m_confirmIndex = m_messages.size();
}

/////////////////////////////////////////////////////////////////////////
/// \brief MessageWidget
///

MessageWidget *MessageWidget::m_instance = 0;

MessageWidget::MessageWidget(QWidget *parent)
    : QWidget(parent, Qt::Tool)
    , ui(new Ui::MessageWidget)
{
    ui->setupUi(this);

    m_toggleAction = new QAction(QString::fromLocal8Bit("显示/隐藏信息"), this);
    m_toggleAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
    m_toggleAction->setCheckable(true);
    connect(m_toggleAction, SIGNAL(toggled(bool)), this, SLOT(setVisible(bool)));

    m_messages = new MessageModel(this);
    ui->view->setModel(m_messages);

    m_delayHideTimer = new QTimer(this);
    m_delayHideTimer->setSingleShot(true);
    m_delayHideTimer->setInterval(10000);
    connect(m_delayHideTimer, SIGNAL(timeout()), this, SLOT(hide()));

    m_instance = this;
}

MessageWidget::~MessageWidget()
{
    m_instance = 0;
    delete ui;
}

MessageWidget *MessageWidget::instance()
{
    if (!m_instance)
        return new MessageWidget;
    else
        return m_instance;
}

void MessageWidget::showEvent(QShowEvent *)
{
    m_toggleAction->setChecked(true);
    QDesktopWidget *desktop = qApp->desktop();
    QRect rect = desktop->availableGeometry();
    move(rect.right() - frameGeometry().width(), rect.bottom() - frameGeometry().height());
}

void MessageWidget::hideEvent(QHideEvent *)
{
    m_toggleAction->setChecked(false);
}

void MessageWidget::showMessage(Workbench *wb, int channel, int type, const QString &text, bool show)
{
    if (!m_messages->showMessage(wb, channel, type, text, show))
        return;

    QScrollBar *vbar = ui->view->verticalScrollBar();
    vbar->setValue(vbar->maximum());

    if (show)
    {
        this->show();
        m_delayHideTimer->stop();
        if (!ui->mute->isChecked() && type > QtWarningMsg)
            qApp->playSound();
    }
    else
    {
        if (!m_messages->hasWarnings())
        {
            m_delayHideTimer->start();
            qApp->stopSound();
        }
    }
}

void MessageWidget::on_mute_toggled(bool checked)
{
    if (checked)
        qApp->stopSound();
}

void MessageWidget::on_confirm_clicked()
{
    m_messages->confirm();
    qApp->stopSound();
}

void MessageWidget::on_wordWrap_toggled(bool checked)
{
    ui->view->setWordWrap(checked);
}
