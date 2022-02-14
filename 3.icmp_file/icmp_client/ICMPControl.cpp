
#include <QDebug>
#include "ICMPControl.h"
#pragma comment(lib, "WS2_32")

//IP头部
#pragma pack(1)
typedef struct IPHeader{
    char chVerHlen; //4位版本+4位首部长度
    char chTOS;
    USHORT usTotalLen;
    USHORT usID;
    USHORT usFlagFragOffset;
    char chTTL;
    char chProtocol;
    USHORT usChecksum;
    unsigned int uisrcip;
    unsigned int uidesip;
}IPHeader;
#pragma pack()

//ICMP头
#pragma pack(1)
typedef struct ICMPHeader{
    char chType;        //类型
    char chCode;        //代码
    USHORT usChecksum;  //校验和
    USHORT usID;        //标识符
    USHORT usSeq;       //序列号
    //unsigned int uiTimeStamp; //时间戳
}ICMPHeader;
#pragma pack()

#define DATA_SIZE 1024 //承载的数据包大小
#define CLIENT_ID 0x125

CICMPControl::CICMPControl()
{
    m_uiSeq = 0;
}

bool CICMPControl::Init()
{
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(1,1), &wsadata) != 0)
    {
        return false;
    }

    m_sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    m_psendbuffer = (char*)malloc(DATA_SIZE + sizeof (ICMPHeader));
}

bool CICMPControl::SendData(char *pdata, int ilen, std::string strIP)
{
    if (ilen > DATA_SIZE) return false;

    int isendlen = sizeof (ICMPHeader) + ilen;

    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.S_un.S_addr = inet_addr(strIP.c_str());
    sockaddr.sin_port = htons(6000);

    ICMPHeader *pHeader = (ICMPHeader*)m_psendbuffer;
    pHeader->chType = 8;
    pHeader->chCode = 0;
    pHeader->usID = CLIENT_ID;
    pHeader->usSeq = m_uiSeq++;
    //pHeader->uiTimeStamp = ::GetTickCount();
    memcpy(m_psendbuffer+sizeof (ICMPHeader), pdata, ilen);
    pHeader->usChecksum = 0;
    pHeader->usChecksum = CalCheckSum((USHORT*)m_psendbuffer, isendlen);

    //阻塞式发送
    ::sendto(m_sock, m_psendbuffer, isendlen, 0, (const struct sockaddr*)&sockaddr, sizeof (sockaddr_in));

}

int CICMPControl::RecvData(char *pdata, int idatalen)
{
    timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 200;
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(m_sock, &fds);

    int iret = select(0, &fds, 0, 0, &tv);

    if (iret == 0)
    {
        //qDebug() << "continue";
        return -1;
    }

    if (FD_ISSET(m_sock, &fds))
    {
        //char chbuffer[DATA_SIZE*2] = {0};
        struct sockaddr sockaddr;int ilen;
        //int i = ::recvfrom(m_sock, pdata, idatalen, 0, (struct sockaddr*)&sockaddr, &ilen);
        int i = ::recv(m_sock, pdata, idatalen, 0); //上面那个收不到,总是返回-1
        //qDebug() << i;
        if (i == SOCKET_ERROR)
        {
            qDebug() << "GetLastError:" << WSAGetLastError();
        }
        return i;
    }
    return -1;
}

//这里iSize为字节长度，不是几个ushort
USHORT CICMPControl::CalCheckSum(USHORT *pBuffer, int iSize)
{
    UINT32 ret = 0;
    while(iSize > 1){
        ret += *pBuffer++; //一次++偏移两个字节。
        iSize -= 2; //注意
    }
    if (iSize == 1){ //如果奇数个，最后一个USHORT中，高字节有效
        ret += *(UCHAR*)pBuffer;
    }

    while (ret >> 16){
        ret = (ret >> 16) + (ret & 0xffff); //高16位与低16位相加，直到高16位为0退出
    }
    ret = ~ret; //最后取反

    return ret;

}
