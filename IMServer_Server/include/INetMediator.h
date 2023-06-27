#pragma once
#include<INet.h>
class INet;
class INetMediator
{
public:
	INetMediator() {};
	virtual ~INetMediator() {};
	//打开网络
	virtual bool OpenNet() = 0;
	//关闭网络
	virtual void CloseNet() = 0;
	//发送数据
	virtual bool SendData(long lSendIP, char* buf, int len) = 0;
	//处理数据
	virtual void DealData(long lSendIP, char* buf, int len) = 0;

	INet* m_pNet;

};
