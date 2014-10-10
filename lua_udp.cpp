#include<iostream>
#include<string>
#include "socket_client.h"

extern "C" int udp_send(char* inbuff, int inbufflen, char* outbuff, const char* ip, int port, int waits)
{
    int result = 0;
    SocketClient *sock = new SocketClient(ip,port);
    if( sock->ini_errno < 0)
    {
        return -1;
    }
    //发包
    int sendbak = sock->send_data(inbuff, inbufflen);
    if(sendbak < 0)
    {
        return -2;
    }
    //收包  
    int revbak = sock->recv_data(waits, outbuff, 8000);
    if(revbak > 0)
    {
        return revbak;
    }
    return -3;
}
