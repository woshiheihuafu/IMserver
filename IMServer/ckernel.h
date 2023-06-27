#ifndef CKERNEL_H
#define CKERNEL_H
#include"INetMediator.h"
#include <QObject>
#include"imserverdialog.h"
#include"packet.h"
#include"logindialog.h"
#include<QTextCodec>
#include"useritem.h"
#include<Qmap>
#include"chatdialog.h"

//定义函数指针
class CKernel;  //声明有CKernel 这个类
typedef void (CKernel::*FUN)(long lSendlp,char* buf,int nLen);

class CKernel : public QObject
{
    Q_OBJECT
public:
    explicit CKernel(QObject *parent = 0);
    ~CKernel();
    //QString UTF-8 -> char* gb2312
    static void utf8ToPgb2312(QString &utf8,char* gbBuf,int nLen);
    //char* gb2312 ->QString UTF-8
    static QString pgb2312ToUtf8(char* gbBuf);
    //初始化协议映射表函数
    void setProtocalMap();
    //处理注册回复
    void dealRegisterRs(long lSendlp,char* buf,int nLen);
    //处理登录回复
    void dealLoginRs(long lSendlp,char* buf,int nLen);
    //处理好友信息请求
    void dealFriendInfoRq(long lSendlp,char* buf,int nLen);
    //处理服务端转发过来的聊天请求
    void dealChatRq(long lSendlp,char* buf,int nLen);
    //处理服务端发过来的聊天回复
    void dealChatRs(long lSendlp,char* buf,int nLen);
    //处理好友请求
    void dealAddFriendRq(long lSendlp,char* buf,int nLen);
    //处理添加好友回复
    void dealAddFriendRs(long lSendlp,char* buf,int nLen);
signals:

public slots:
    //接收总数据 的处理槽函数
   void slot_ReadyData(long lSendlp,char* buf,int nLen);
    //注册信息处理槽函数
   void slot_registerCommit(QString name,QString tel,QString password);
   //登录信息处理槽函数
   void slot_loginCommit(QString tel,QString password);
    //useritem 点击处理槽函数
   void slot_userItemClicked(int id);
   //用户发送聊天内容的处理槽函数
   void slot_sendChatMsg(int id,QString content);
   //添加好友的信息处理槽函数
   void slot_AddFriend();
private:
   //保存自己的id
   int m_id;
   //保存自己的昵称
   QString m_name;
   //保存自己的头像
   int m_icon;
   //保存自己的个性签名
   QString m_feel;
   //中介者类对象
   INetMediator* m_pMediator;
   //好友列表界面对象
   IMServerDialog* m_MainWnd;
   //注册&登录界面
   LoginDialog * m_pLoginDialog;
   //协议映射表，协议头映射处理函数
    FUN  m_netProtocalMap[_DEF_PROTOCAL_COUNT];
    //管理id---useritem
    QMap<int,useritem*> m_mapIdToUserItem;
    //管理id->聊天窗口
    QMap<int,ChatDialog*>m_mapIdToChatDialog;
};

#endif // CKERNEL_H
