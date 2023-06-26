#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QTime>
ChatDialog::ChatDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatDialog)
{
    ui->setupUi(this);
    QString con = QString("[%1] %2").arg("m_name")
           .arg(QTime::currentTime().toString("hh:mm:ss"));
    QString content = "哈鲁";
    ui->plainTextEdit->appendPlainText(con);
    ui->plainTextEdit->appendPlainText(content);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}
//设置窗口信息
void ChatDialog::setInfo(QString name,int id)
{
    m_name = name;
    m_id = id;
    //设置窗口标题,格式：与【ip】的聊天窗口
    setWindowTitle(QString("与[%1]的聊天窗口").arg(m_name));
}
//设置接收到的聊天内容
void ChatDialog::slot_setChatMsg(QString content)
{
    //格式 [ip] 时间 内容
    QString con = QString("[%1] %2").arg(m_name)
           .arg(QTime::currentTime().toString("hh:mm:ss"));
    ui->tb_chat->append(con);
    ui->tb_chat->append(content);
}
//设置聊天对象不在线
void ChatDialog::slot_setUserOffline()
{
    //格式；[昵称] 时间 用户不在线
    QString content = QString("[%1] %2 用户不在线").arg(m_name).arg(QTime::currentTime().toString("hh:mm:ss"));
    ui->tb_chat->append(content);
}

void ChatDialog::on_pb_send_clicked()
{
    //1.取出用户输入的字符串，判断不为空
    QString content = ui->textEdit->toPlainText();//不带格式 纯文本
    if(content.isEmpty())
        return;
    //2.清空编辑窗口
    content = ui->textEdit->toHtml();//带格式
    ui->textEdit->clear();
    //3.显示到聊天窗口   格式：【我】 时间 内容
    ui->tb_chat->append(QString("[我] %1")
                        .arg(QTime::currentTime().toString("hh:mm:ss")));
    ui->tb_chat->append(content);
    //4.通过信号将字符串发送到kernel
    Q_EMIT SIG_sendChatMsg(m_id,content);
}

void ChatDialog::on_pb_chatHistory_clicked()
{
    //TODO:
}
