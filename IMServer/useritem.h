#ifndef USERITEM_H
#define USERITEM_H

#include <QWidget>

namespace Ui {
class useritem;
}

class useritem : public QWidget
{
    Q_OBJECT
signals:
    //发送点击头像信号
    void SIG_userItemClicker(int id);
public:
    explicit useritem(QWidget *parent = 0);
    ~useritem();
    //设置用户信息
    void slot_setInfo(int id,QString name,int state,int icon,QString feeling);

public:
        int m_state;
            QString m_name;
private slots:
        void on_pb_icon_clicked();

private:
    Ui::useritem *ui;
    int m_id;
    int m_icon;
    QString m_feeling;
};

#endif // USERITEM_H
