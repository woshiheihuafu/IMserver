#pragma once
#include<QObject>

class INet;
class INetMediator:public QObject
{
    Q_OBJECT
public:
    INetMediator();
    virtual ~INetMediator();//qt中不允许在头文件类中实现 所以要加一个cpp文件
	//打开网络
	virtual bool OpenNet() = 0;
	//发送数据
	virtual bool SendData(long lSendlp,char* buf,int nLen) = 0;
	//关闭网络
	
	virtual void CloseNet() = 0;
	//处理数据
	virtual void DealData(long lSendlp,char* buf,int nLen) = 0;
	INet * m_pNet;//网络接口指针，用来调用网络函数
signals:
    void SIG_ReadyData(long lSendlp,char* buf,int nLen);
};

