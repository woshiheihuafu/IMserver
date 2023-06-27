#pragma once
#include<INet.h>
class INet;
class INetMediator
{
public:
	INetMediator() {};
	virtual ~INetMediator() {};
	//������
	virtual bool OpenNet() = 0;
	//�ر�����
	virtual void CloseNet() = 0;
	//��������
	virtual bool SendData(long lSendIP, char* buf, int len) = 0;
	//��������
	virtual void DealData(long lSendIP, char* buf, int len) = 0;

	INet* m_pNet;

};
