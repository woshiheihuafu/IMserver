#include "ckernel.h"
#include "TcpClientMediator.h"
#include <QMessageBox>
#include <QDebug>
#include"useritem.h"
#include<QInputDialog>
//计算协议表数组下标的宏
#define NetPckFunMap(a) m_netProtocalMap[a - _Def_PROTOCAL_BASE - 10]
//初始化协议映射表
void CKernel::setProtocalMap()
{
    memset(m_netProtocalMap,0,sizeof(m_netProtocalMap));
    //绑定协议头和处理函数
    NetPckFunMap(_DEF_TCP_REGISTER_RS) = &dealRegisterRs;
    NetPckFunMap(_DEF_TCP_LOGIN_RS) = &dealLoginRs;
    NetPckFunMap(_DEF_TCP_FRIEND_INFO) = &dealFriendInfoRq;
    NetPckFunMap(_DEF_TCP_CHAT_RQ) = &dealChatRq;
    NetPckFunMap(_DEF_TCP_CHAT_RS) = &dealChatRs;
    NetPckFunMap(_DEF_TCP_ADD_FRIEND_RQ) = &dealAddFriendRq;
    NetPckFunMap(_DEF_TCP_ADD_FRIEND_RS) = &dealAddFriendRs;
}

CKernel::CKernel(QObject *parent) : QObject(parent),m_id(0)
{

    setProtocalMap();
    //显示登录&注册界面
    m_pLoginDialog = new LoginDialog;
    m_pLoginDialog->showNormal();

    //绑定发送注册信息的信号和槽函数
    connect(m_pLoginDialog,SIGNAL(SIG_registerCommit(QString,QString,QString)),
           this,SLOT(slot_registerCommit(QString,QString,QString)));
    //绑定发送登录信息的信号和槽函数
    connect(m_pLoginDialog,SIGNAL(SIG_loginCommit(QString,QString)),
            this,SLOT(slot_loginCommit(QString,QString)));
    //初始化主窗口
    m_MainWnd = new IMServerDialog;
    //m_MainWnd->show();
    //绑定添加好友的信号和槽函数
    connect(m_MainWnd,SIGNAL(SIG_AddFriend()),
            this,SLOT(slot_AddFriend()));
    //初始化中介者类
    m_pMediator = new TcpClientMediator;

    if(!m_pMediator->OpenNet())
    {
        QMessageBox::about(m_MainWnd,"提示","打开网络失败");
        exit(0);
    }
    //绑定发送总数据和处理槽函数
    connect(m_pMediator,SIGNAL(SIG_ReadyData(long,char*,int)),
            this,SLOT(slot_ReadyData(long,char*,int)));



}


CKernel::~CKernel()
{
    //回收资源
    //1.回收中介者类
    if(m_pMediator)
    {
        m_pMediator->CloseNet();
        delete m_pMediator;
        m_pMediator = NULL;
    }
    //2.回收主窗口
    if(m_MainWnd)
        m_MainWnd->hide();
    delete m_MainWnd;
    m_MainWnd = NULL;
    //3.回收登录&注册窗口
    if(m_pLoginDialog)
        m_pLoginDialog->hide();
    delete m_pLoginDialog;
    m_pLoginDialog = NULL;
}

 //QString UTF-8 -> char* gb2312
void CKernel::utf8ToPgb2312(QString &utf8, char *gbBuf, int nLen)
{
    //转码对象
    QTextCodec *gb2312code = QTextCodec::codecForName("gb2312");
    //QByteArray 是char类型数组的封装类
    QByteArray ba = gb2312code->fromUnicode(utf8);
    strcpy_s(gbBuf,nLen,ba.data());
}
//char* gb2312 ->QString UTF-8
QString CKernel::pgb2312ToUtf8(char *gbBuf)
{

    //转码对象
    QTextCodec *gb2312code = QTextCodec::codecForName("gb2312");
    return gb2312code->toUnicode(gbBuf);
}

//接受总数据处理的槽函数
void CKernel::slot_ReadyData(long lSendIp,char* buf,int nLen)
{
    qDebug() << __func__;
    //1.取出协议头
    PackType nType = *(PackType*)buf;
    //根据协议头,判断走哪个处理流程
    //2.判断协议头是否在有效范围内
    if(nType >= _Def_PROTOCAL_BASE + 10 &&
       nType <= _Def_PROTOCAL_BASE + 10 + _DEF_PROTOCAL_COUNT)
    {
        //3.计算数组下标
        FUN pFun = NetPckFunMap(nType);

        //4.如果指针存在，调用处理函数
        if(pFun)
        {
            (this->*pFun)(lSendIp,buf,nLen);
        }
    }
    //5.回收buf(buf是new出来的)
    delete []buf;

}
//注册信息处理槽函数
void CKernel::slot_registerCommit(QString name, QString tel, QString password)
{
    qDebug() << __func__ ;
    //1.类型转换，QString->std::string
//    std::string nameStr = name.toStdString();
    std::string telStr = tel.toStdString();
    std::string passwordStr = password.toStdString();

    //2.封包——注册请求
    STRU_TCP_REGISTER_RQ rq;
//    strcpy(rq.name,nameStr.c_str());
    utf8ToPgb2312(name, rq.name,sizeof(rq.name));
    strcpy(rq.tel,telStr.c_str());
    strcpy(rq.password,passwordStr.c_str());
    //3.发送注册请求给服务端
    m_pMediator->SendData(0,(char*)&rq,sizeof(rq));

}
//登录信息处理槽函数
void CKernel::slot_loginCommit(QString tel, QString password)
{
    qDebug() << __func__ ;
    //1.类型转换，QString->std::string
    std::string telStr = tel.toStdString();
    std::string passwordStr = password.toStdString();

    //2.封包——登录请求
    STRU_TCP_LOGIN_RQ rq;
    strcpy(rq.tel,telStr.c_str());
    strcpy(rq.password,passwordStr.c_str());
    //3.发送注册请求给服务端
    m_pMediator->SendData(0,(char*)&rq,sizeof(rq));
}
//useritem 点击处理槽函数
void CKernel::slot_userItemClicked(int id)
{
    //判断与该好友的聊天窗口是否存在
    if(m_mapIdToChatDialog.find(id) != m_mapIdToChatDialog.end())
    {
        ChatDialog* chat = m_mapIdToChatDialog[id];
        chat->showNormal();
    }
}

//用户发送聊天内容的处理槽函数
void CKernel::slot_sendChatMsg(int id, QString content)
{

    qDebug()<<__func__;
    //1.封包
    STRU_TCP_CHAT_RQ rq;
    //聊天内容不需要编码转换，因为聊天内容在服务器和数据库都不使用
    //2.赋值
    rq.userId = m_id;
    rq.friendId = id;
    strcpy(rq.content,content.toStdString().c_str());
    //3.发送给服务器
    m_pMediator->SendData(0,(char*)&rq,sizeof(rq));//为什么是0就一个服务器，底层已经保存了
}
//添加好友的处理槽函数
void CKernel::slot_AddFriend()
{
    qDebug()<<__func__;
    //1.弹出一个添加好友窗口
    QString strName = QInputDialog::getText(m_MainWnd,"添加好友","请输入好友昵称");
    //2.校验用户输入的昵称是否合法
    if(strName.isEmpty() || strName.length() >10)
    {
        QMessageBox::about(m_MainWnd,"昵称","输入不合法");
        return;
    }
    //3.判断是不是自己，如果是自己，就不用发给服务端了
    if(strName == m_name)//char数组要用strcmp 比较，为什么这个不需要，因为qstring重载了==
    {
        QMessageBox::about(m_MainWnd,"提示","您不能添加自己为好友");
        return;
    }
    //4.判断是否已经是好友了,如果已经是好友了，就不用发给服务端
    for(auto ite = m_mapIdToUserItem.begin() ; ite != m_mapIdToUserItem.end(); ite++)
    {
         useritem *item = *ite;
         if(item->m_name == strName)
         {
             QMessageBox::about(m_MainWnd,"提示","已经与该用户是好友，不需要再添加");
             return;
         }
    }
    //5.封包，发给服务端
    STRU_TCP_ADD_FRIEND_RQ rq;
    rq.userId = m_id;
    //A添加B为好友，A的昵称只在B客户端显示需要，不需要在服务端使用，所以不用转码
    strcpy(rq.userName,m_name.toStdString().c_str());//给char数组赋值
    //B的昵称需要在数据库查询，所以需要编码转换，utf8->qb2312
    //rq.friendName = strName;
    utf8ToPgb2312(strName,rq.friendName,sizeof(rq.friendName));
    //6.发送给服务端
    m_pMediator->SendData(0,(char*)&rq,sizeof(rq));
}

//处理注册回复
void CKernel::dealRegisterRs(long lSendlp, char *buf, int nLen)
{
    qDebug() << __func__ ;
    //1.拆包
    STRU_TCP_REGISTER_RS *rs = (STRU_TCP_REGISTER_RS*)buf;
    //2.取出注册结果，提示用户
    switch(rs->result)
    {
    case user_is_exit:
        QMessageBox::about(m_pLoginDialog,"提示","注册失败，用户已经存在");
        break;
    case parameter_error:
        QMessageBox::about(m_pLoginDialog,"提示","注册失败，注册信息不符合要求");
        break;
    case register_fail:
        QMessageBox::about(m_pLoginDialog,"提示","注册失败，数据库更新失败");
        break;
    case register_success:
        QMessageBox::about(m_pLoginDialog,"提示","注册成功");
        break;

    }
    //
}
//处理登录回复
void CKernel::dealLoginRs(long lSendlp, char *buf, int nLen)
{
    qDebug() << __func__ ;
    qDebug() << __func__ ;
    //1.拆包
    STRU_TCP_LOGIN_RS *rs = (STRU_TCP_LOGIN_RS*)buf;
    //2.取出注册结果，提示用户
    switch(rs->result)
    {
    case parameter_login_error:
        QMessageBox::about(m_pLoginDialog,"提示","tel，密码格式错误");
        break;
    case user_not_exist:
        QMessageBox::about(m_pLoginDialog,"提示","用户不存在");
        break;
    case password_error:
        QMessageBox::about(m_pLoginDialog,"提示","密码错误");
        break;
    case login_success:
//            QMessageBox::about(m_pLoginDialog,"提示","登录成功");
        {
            //隐藏登录界面，显示好友界面
            m_pLoginDialog->hide();
            m_MainWnd->showNormal();
            //保存用户id
            m_id = rs->userId;
            break;
        }

    }

}
//处理好友信息请求
void CKernel::dealFriendInfoRq(long lSendlp, char *buf, int nLen)
{
    qDebug() << __func__ ;
    //1.拆包
    STRU_TCP_FRIEND_INFO *rq = (STRU_TCP_FRIEND_INFO*)buf;
    //1.1昵称和签名编码转换
    QString nameTemp = pgb2312ToUtf8(rq->name);
    QString feelingTemp = pgb2312ToUtf8(rq->feling);

    //2.判断是不是自己的信息
            //2.1如果是自己的信息，更新用户界面，然后返回
    if(m_id == rq->userId)
    {
        m_MainWnd->slot_setInfo(nameTemp,feelingTemp,rq->iconId);
        //保存用户昵称
        m_name = nameTemp;
        return;
    }
    //3.查看好友列表是否已经由在这个好友
    //4.如果没有这个好友
    if(m_mapIdToUserItem.find(rq->userId) == m_mapIdToUserItem.end())
    {
        //4.1就new一个useritem添加到好友列表里面
        useritem *item = new useritem;
        item->slot_setInfo(rq->userId,nameTemp,rq->state,rq->iconId,feelingTemp);
        //4.2绑定useritem 点击信号和槽函数
        connect(item,SIGNAL(SIG_userItemClicker(int)),this,SLOT(slot_userItemClicked(int)));
        //4.3创建聊天窗口
         ChatDialog* chat = new ChatDialog;
         //4.4绑定用户发送聊天内容的信号和槽函数
         connect(chat,SIGNAL(SIG_sendChatMsg(int,QString)),this,SLOT(slot_sendChatMsg(int,QString)));
        chat->setInfo(nameTemp,rq->userId);
        //4.5聊天窗口放在map中管理
        m_mapIdToChatDialog[rq->userId] = chat;
        //4.6添加到好友列表
        m_MainWnd->slot_addFriend(item);
        //4.7添加到map中管理
        m_mapIdToUserItem[rq->userId] = item;
    }
    //5.如果已经有这个好友，更新好友信息
    else
    {
        //5.1取出useritem
        useritem *item = m_mapIdToUserItem[rq->userId];
        //5.2如果好友之前是下线状态，现在是上线状态，提示一下
        if(item && item->m_state == 0 && rq->state == 1)
        {
            QMessageBox::about(m_MainWnd,"提示",QString("用户[%l]已上线").arg(nameTemp));
        }
        //5.3更新好友信息
        item->slot_setInfo(rq->userId,nameTemp,rq->state,rq->iconId,feelingTemp);
    }
}
//处理服务端转发过来的聊天请求
void CKernel::dealChatRq(long lSendlp,char* buf,int nLen)
{
    qDebug()<<__func__;
    //1.拆包
    STRU_TCP_CHAT_RQ *rq = (STRU_TCP_CHAT_RQ*) buf;
    //2.查看与该用户的聊天窗口是否存在
    if(m_mapIdToChatDialog.find(rq->userId) != m_mapIdToChatDialog.end())
    {
        //3.聊天窗口存在，就显示出来，并且设置用户聊天信息，并显示出来
        ChatDialog* chat = m_mapIdToChatDialog[rq->userId];
        chat->slot_setChatMsg(rq->content);
        chat->showNormal();
    }
}
//处理服务端发过来的聊天回复
void CKernel::dealChatRs(long lSendlp,char* buf,int nLen)
{
    qDebug()<<__func__;
    //1.拆包
    STRU_TCP_CHAT_RS *rs = (STRU_TCP_CHAT_RS*) buf;
    //2.查看与该用户的聊天窗口是否存在
    if(m_mapIdToChatDialog.find(rs->friendId) != m_mapIdToChatDialog.end())
    {
        //3.聊天窗口存在，就显示出来，并且设置用户不在线信息，并显示出来
        ChatDialog* chat = m_mapIdToChatDialog[rs->friendId];
        chat->slot_setUserOffline();
        chat->showNormal();
    }

}
//处理添加好友请求
void CKernel::dealAddFriendRq(long lSendlp, char *buf, int nLen)
{
    qDebug()<<__func__;
    //1.拆包
    STRU_TCP_ADD_FRIEND_RQ *rq = (STRU_TCP_ADD_FRIEND_RQ*)buf;
    //2.定义添加好友的回复包
    STRU_TCP_ADD_FRIEND_RS rs;
    rs.friendId = m_id;
    rs.userId = rq->userId;
    strcpy(rs.friendName,m_name.toStdString().c_str());
    //3.弹窗询问用户是否同意添加好友
    QString str = QString("用户[%1]请求添加你为好友，是否同意?").arg(rq->userName);
    if(QMessageBox::Yes == QMessageBox::question(m_MainWnd,"添加好友",str))
    {
        //同意添加好友
        rs.result = add_success;
    }
    else
    {
        //不同意
        rs.result = user_refuse;
    }
    //4.发送添加还有回复包给服务器
    m_pMediator->SendData(0,(char*)&rs,sizeof(rs));
}
//处理添加好友回复
void CKernel::dealAddFriendRs(long lSendlp, char *buf, int nLen)
{
    qDebug()<<__func__;
    //1.拆包
    STRU_TCP_ADD_FRIEND_RS *rs = (STRU_TCP_ADD_FRIEND_RS*)buf;
    //2.好友昵称需要转码
    QString strName = pgb2312ToUtf8(rs->friendName);
    //2.取出结果，显示信息
    switch(rs->result)
    {
    case add_fail:
        QMessageBox::about(m_MainWnd,"提示","添加好友失败");
        break;
    case user_offline:
        QMessageBox::about(m_MainWnd,"提示",QString("用户[%1]不在线,添加好友失败").arg(rs->friendName));
        break;
    case user_refuse:
        QMessageBox::about(m_MainWnd,"提示",QString("用户[%1]拒绝添加好友").arg(strName));
        break;
    case no_this_user:
        QMessageBox::about(m_MainWnd,"提示",QString("用户[%1]不存在").arg(rs->friendName));
        break;
    case add_success:
        QMessageBox::about(m_MainWnd,"提示",QString("添加用户[%1]成功").arg(strName));
        break;

    }
}
