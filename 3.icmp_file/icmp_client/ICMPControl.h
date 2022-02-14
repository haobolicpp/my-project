#ifndef CICMPCONTROL_H
#define CICMPCONTROL_H
#include <WinSock2.h>
#include <string>

class CICMPControl
{
public:
    CICMPControl();

    bool Init();

    bool SendData(char *pdata, int ilen, std::string strIP);
    int RecvData(char *pdata, int idatalen);

private:
    SOCKET m_sock;
    char *m_psendbuffer;
    unsigned short m_uiSeq;

private:
    USHORT CalCheckSum(USHORT *pBuffer, int iSize);
};

#endif // CICMPCONTROL_H
