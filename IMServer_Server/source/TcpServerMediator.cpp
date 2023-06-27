#include<TcpServerMediator.h>
#include<TcpServer.h>
#include <CKernel.h>
TcpServerMediator::TcpServerMediator()
{
	m_pNet = new TcpServer(this);
}
TcpServerMediator::~TcpServerMediator()
{
	if (m_pNet)
	{
		delete m_pNet;
		m_pNet = nullptr;
	}
}
bool TcpServerMediator::OpenNet()
{
	if (m_pNet->InitNet())
		return true;
	return false;
}
void TcpServerMediator::CloseNet()
{
	m_pNet->UnInitNet();
}
bool TcpServerMediator::SendData(long lSendIP, char* buf, int len)
{
	if (!m_pNet->SendData(lSendIP, buf, len))
	{
		return false;
	}
	return true;
}
void TcpServerMediator::DealData(long lSendIP, char* buf, int len)
{
	//传给kernel类去处理
	CKernel::m_kernel->dealData(lSendIP,buf,len);
}
