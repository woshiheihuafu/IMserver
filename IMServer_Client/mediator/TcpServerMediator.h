#include"INetMediator.h"

class TcpServerMediator:public INetMediator
{
    Q_OBJECT
public:
	TcpServerMediator();
	~TcpServerMediator();
	//打开网络
	bool OpenNet();
	//发送数据
	bool SendData(long ISendIp,char* buf,int nLen);
	//关闭网络
	void CloseNet();
	
	void DealData(long ISendIp,char* buf,int nLen);
};
