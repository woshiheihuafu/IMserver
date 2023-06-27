#pragma once
#include<config.h>
#include<iostream>
#include<INetMediator.h>
using namespace std;
class INetMediator;
class INet
{
public:
	INet() {};
	virtual ~INet() {};
	//初始化网络
	virtual bool InitNet() = 0;
	//关闭初始化网络
	virtual void UnInitNet() = 0;
	//发送数据
	virtual bool SendData(long lSendIP,char* buf,int len) = 0;
public://仅仅在类内调用的尽量protected
	//接收数据
	virtual void RecvData() = 0;
	INetMediator* m_pMediator;

};
