#ifndef MAINPAGEWIDGET_H
#define MAINPAGEWIDGET_H

#include <QObject>
#include <QWidget>

class Workbench;
class QMenu;
class QLabel;
class QModelIndex;

namespace Ui
{
    class MainPageWidget;
}

class MainPageWidget : public QWidget
{
    Q_OBJECT

public:
    MainPageWidget(Workbench *wb, QWidget *parent = 0);
    ~MainPageWidget();

    Workbench *workbench() const { return m_workbench; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_screen_full();             //全屏模式
    void on_screen_normal();           //普通模式

    void on_show_video_1();            //切换到1画面
    void on_show_video_4();            //切换到4画面
    void on_show_video_9();            //切换到9画面
    void on_show_video_16();           //切换到16画面

private:
    void initVideo();
    void initInfoTable();

    void removelayout();            //移除所有布局
    void changeVideo1(int index);   //改变1画面布局
    void changeVideo2(int index);   //改变2画面布局
    void changeVideo4(int index);   //改变4画面布局
    void changeVideo9(int index);   //改变9画面布局
    void changeVideo16(int index);  //改变16画面布局

    void changeVideoLayout();
    void loadVideo();

private:
    Workbench *m_workbench;

    Ui::MainPageWidget *ui;

    QList<QLabel *> VideoLab;       //通道显示视频lab载体
    QList<QLayout *> VideoLay;      //通道视频所在lab的layout

    bool m_videoMax;                //通道是否处于最大化
    QString m_VideoType;            //相机界面数
};

#endif // MAINPAGEWIDGET_H
