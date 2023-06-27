#pragma once

#include"INetMediator.h"
class TcpServerMediator :public INetMediator
{
public:
	TcpServerMediator();
	~TcpServerMediator();
	bool OpenNet();
	void CloseNet();
	bool SendData(long lSendIP, char* buf, int len);
	void DealData(long lSendIP, char* buf, int len);

};