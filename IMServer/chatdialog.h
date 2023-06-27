#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QWidget>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QWidget
{
    Q_OBJECT
signals:
    //发送聊天内容的信号
    void SIG_sendChatMsg(int id,QString content);
public:
    explicit ChatDialog(QWidget *parent = 0);
    ~ChatDialog();
    //设置窗口信息
    void setInfo(QString name,int id);
    //设置接受到的聊天内容
    void slot_setChatMsg(QString content);
    //设置聊天对象不在线
    void slot_setUserOffline();
private slots:
    void on_pb_send_clicked();

    void on_pb_chatHistory_clicked();

private:
    Ui::ChatDialog *ui;
    int m_id;
    QString m_name;
};

#endif // CHATDIALOG_H
