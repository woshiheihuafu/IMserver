#pragma once
#include"INet.h"

class UdpNet:public INet
{
public:
	UdpNet(INetMediator* pMediator);
	~UdpNet();	
	//��ʼ������
	bool InitNet();
	//�ر�����
	void UninitNet();
	//��������
	bool SendData(long ISendIp,char* buf,int nLen );
protected:
	//��������
	void RecvData();//ֻҪ�ⲿ�����þ������ protected
	SOCKET m_sock;
	bool m_isStop;
	HANDLE m_hThreadHandle;
	static unsigned int _stdcall RecvThrad(void* lpvoid);
	
};