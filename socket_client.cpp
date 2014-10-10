//*******************************************************************************
// File name: socket_client.cpp
// Create date: 2013/05/30
// Author: xingzheng
// Description: socket_client实现文件，定义socket的收发包
//				
//*******************************************************************************
#include "socket_client.h";
#include <iostream>
#include <fstream>
using namespace std;

//初始化socket
SocketClient::SocketClient(const char *ip,unsigned short port)
{
	//初始化socket
	//int sockfd = socket(AF_INET,SOCK_DGRAM,0);
	ini_errno = 0;		
	sockfd = socket(AF_INET,SOCK_DGRAM,0);	
	if(sockfd < 0) 
	{
		ini_errno = -1;		
	}
	m_nfds = sockfd + 1;
	//设置为非阻塞	
    int flags = 1;
	if(ioctl(sockfd, FIONBIO, &flags) && ((flags = fcntl(sockfd, F_GETFL, 0)) < 0 || fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0))
	{
		::close(sockfd);
		ini_errno = -2;		
	}
	
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(port);
	
	FD_ZERO(&m_readfds);	
}

//析构
SocketClient::~SocketClient()
{
	::close(sockfd);
}

void SocketClient::closeSocket()
{
	::close(sockfd);
}

//发包
int SocketClient::send_data(const char* msg,unsigned int len)
{
	FD_SET(sockfd,&m_readfds);
	int ret = sendto(sockfd,msg,len,0,(struct sockaddr *)&servaddr,sizeof(servaddr));
	if(ret < 0)
	{	
		FD_CLR(sockfd,&m_readfds);
		closeSocket();
	}
	return ret;
}


//收包
int SocketClient::recv_data(int waitms,char* buffer,int size)
{
	struct timeval tv;
    tv.tv_sec = waitms / 1000;
    tv.tv_usec = waitms % 1000 * 1000;
	
    int ret = ::select(m_nfds,&m_readfds,NULL,NULL,&tv);
    if(ret <= 0)
    {
		FD_CLR(sockfd,&m_readfds);
		closeSocket();
		return ret;
    }
	
	if(FD_ISSET(sockfd,&m_readfds))
	{
		ret = recvfrom(sockfd,buffer,size,0,NULL,NULL);
		FD_CLR(sockfd,&m_readfds);
		closeSocket();
		return ret;
    }
	else
	{
		FD_CLR(sockfd,&m_readfds);
		closeSocket();
		return 0;
	}
}
