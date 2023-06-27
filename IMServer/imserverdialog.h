#ifndef IMSERVERDIALOG_H
#define IMSERVERDIALOG_H

#include <QMainWindow>
#include<QVBoxLayout>
#include"useritem.h"
#include<QMenu>
namespace Ui {
class IMServerDialog;
}

class IMServerDialog : public QMainWindow
{
    Q_OBJECT

signals:
    void SIG_AddFriend();
public:
    explicit IMServerDialog(QWidget *parent = 0);
    ~IMServerDialog();
    //添加好友
    void slot_addFriend(useritem *item);
    //设置界面信息
    void slot_setInfo(QString name,QString feeling,int icon);

private slots:
    void on_pb_menu_clicked();
    //处理菜单点击事件的槽函数
    void slot_dealMenu(QAction * action);

private:
    Ui::IMServerDialog *ui;
    //垂直布局的层
    QVBoxLayout *m_layout;
    //菜单
    QMenu *m_menu;
};

#endif // IMSERVERDIALOG_H
