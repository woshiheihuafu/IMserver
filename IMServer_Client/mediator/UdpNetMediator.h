#include"INetMediator.h"

class UdpNetMediator:public INetMediator
{
public:
	UdpNetMediator();
	~UdpNetMediator();
	//������
	bool OpenNet();
	//��������
	bool SendData(long ISendIp,char* buf,int nLen);
	//�ر�����
	void CloseNet();
	
	void DealData(long ISendIp,char* buf,int nLen);
};