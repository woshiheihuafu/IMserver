#pragma once
#include<WinSock2.h>
#pragma comment(lib,"Ws2_32.lib")
#include<iostream>
using namespace std;
class INetMediator;
class INet
{
public:
    INet(){}
    virtual ~INet(){} //为什么要用虚析构：父类指针指向子类对象，要用虚析构
	//初始化网络
	virtual bool InitNet() = 0;
	//关闭网络
	virtual void UninitNet() = 0;
	//发送数据
	virtual bool SendData(long ISendIp,char* buf,int nLen ) = 0;
protected:
	//接受数据
	virtual void RecvData() = 0;//只要外部不调用尽量设成 protected
	INetMediator* m_pMediator;//中介者接口指针
};
