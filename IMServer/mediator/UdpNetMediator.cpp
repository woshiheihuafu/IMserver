#include"UdpNetMediator.h"
#include"UdpNet.h"
UdpNetMediator::UdpNetMediator()
{
	m_pNet = new UdpNet(this);
}
UdpNetMediator::~UdpNetMediator()
{
	if(m_pNet)
	{
		delete m_pNet;
		m_pNet = NULL;
	}
}
//������
bool UdpNetMediator::OpenNet()
{
	if(!m_pNet->InitNet())
	{
		return false;
	}
	return true;
}
//��������
bool UdpNetMediator::SendData(long lSendlp,char *buf,int nLen)
{
	if(!m_pNet->SendData(lSendlp,buf,nLen))
	{
		return false;
	}
	return true;
}
//�ر�����
void UdpNetMediator::CloseNet()
{
	m_pNet->UninitNet();  
}
//��������
#include<iostream>
void UdpNetMediator::DealData(long ISendIp,char* buf,int nLen)
{
	//TODO:�����ݴ���kernel��
	std::cout<<buf<<std::endl;
}