//********************************************************************************/
// File name: socket_client.h
// Create date: 2013/05/30
// Author: xingzheng
// Description: socket_client头文件，定义socket的收发包
//********************************************************************************/

#ifndef _SOCKET_CLIENT_
#define _SOCKET_CLIENT_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <vector>

class SocketClient
{
public:
    SocketClient(const char *ip,unsigned short port);
    ~SocketClient();
    int send_data(const char* msg,unsigned int len);
    int recv_data(int waitms,char* buffer,int size=10000);
	int ini_errno;
private:
	int sockfd;
	struct sockaddr_in servaddr;
	fd_set m_readfds;
    int m_nfds; //select para 1
	void closeSocket();
};

#endif