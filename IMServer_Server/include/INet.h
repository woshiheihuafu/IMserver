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
	//��ʼ������
	virtual bool InitNet() = 0;
	//�رճ�ʼ������
	virtual void UnInitNet() = 0;
	//��������
	virtual bool SendData(long lSendIP,char* buf,int len) = 0;
public://���������ڵ��õľ���protected
	//��������
	virtual void RecvData() = 0;
	INetMediator* m_pMediator;

};
