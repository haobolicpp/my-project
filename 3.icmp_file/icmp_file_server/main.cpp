#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include "pcap.h"
#pragma comment(lib,"wpcap.lib")
#pragma comment(lib,"packet.lib")
#pragma comment(lib, "WS2_32")

QDataStream *pg_ds = nullptr;
QFile *pg_file = nullptr;

/* prototype of the packet handler */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

#pragma pack(1)
typedef struct TFileHeader
{
    char chEndPackage; //是否是最后一包
    int index; //数据包索引
    int ilength; //该包数据包长度
}TFileHeader;
#pragma pack()
pcap_t *adhandle;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    pcap_if_t *alldevs;
    pcap_if_t *d;
    int inum;
    int i=0;

    char errbuf[PCAP_ERRBUF_SIZE];

    /* Retrieve the device list */
    if(pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }

    printf("the list:\n");
    /* Print the list */
    for(d=alldevs; d; d=d->next)
    {
        struct sockaddr_in *sock = ( struct sockaddr_in*)d->addresses->addr;
        int port = ntohs(sock->sin_port);
        qDebug() << "    " << ++i << d->name << inet_ntoa(sock->sin_addr);

    }

    if(i==0)
    {
        printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
        return -1;
    }

    inum = 2;
    printf("selected the interface number:%d",inum);
    //scanf("%d", &inum);


    if(inum < 1 || inum > i)
    {
        printf("\nInterface number out of range.\n");
        /* Free the device list */
        pcap_freealldevs(alldevs);
        return -1;
    }

    /* Jump to the selected adapter */
    for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);

    /* Open the device */
    /* Open the adapter */
    if ((adhandle= pcap_open_live(d->name,	// name of the device
                                  65536,			// portion of the packet to capture.
                                  // 65536 grants that the whole packet will be captured on all the MACs.
                                  1,				// promiscuous mode (nonzero means promiscuous)
                                  1000,			// read timeout
                                  errbuf			// error buffer
                                  )) == NULL)
    {
        fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
        /* Free the device list */
        pcap_freealldevs(alldevs);
        return -1;
    }

    bpf_u_int32 NetMask=0xffffff;

    //compile the filter
    struct bpf_program fcode;
    const char *filter = "icmp and ip dst 10.60.66.16";
    if(pcap_compile(adhandle, &fcode, filter, 1, NetMask) < 0)
    {
        fprintf(stderr,"\nError compiling filter: wrong syntax.\n");

        pcap_close(adhandle);
        return -3;
    }

    //set the filter
    if(pcap_setfilter(adhandle, &fcode)<0)
    {
        fprintf(stderr,"\nError setting the filter\n");

        pcap_close(adhandle);
        return -4;
    }

    printf("\nlistening on %s...\n", d->description);

    /* At this point, we don't need any more the device list. Free it */
    pcap_freealldevs(alldevs);

    /* start the capture */
    pcap_loop(adhandle, 0, packet_handler, NULL);

    pcap_close(adhandle);


    return a.exec();
}

/* Callback function invoked by libpcap for every incoming packet */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
//    struct tm *ltime;
//    char timestr[16];
//    time_t local_tv_sec;
    /* convert the timestamp to readable format */
//    local_tv_sec = header->ts.tv_sec;
//    ltime=localtime(&local_tv_sec);
//    strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);
//    qDebug() << timestr << header->ts.tv_usec << header->len;

    //一帧数据，包含了ip头和icmp头，大小是34 + 8，后面的数据体部分，不足18个字节的，补满18个字节！
    const u_char *pData = pkt_data + 42;
    TFileHeader *pHeader = (TFileHeader*)pData;
    pData = pData + sizeof (TFileHeader);

    static int ilast = 0;

    printf("recv one frame, index:%d,len:%d\n", pHeader->index, pHeader->ilength);

    if (pg_file == nullptr)
    {
        /*要保存的文件*/
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("hh-mm-ss");
        pg_file = new QFile(QString("e://recv-%1.dat").arg(current_date));
        if (!pg_file->open(QFile::WriteOnly))
        {
            printf("open file failed!");
            return;
        }
        pg_ds = new QDataStream(pg_file);
        ilast = pHeader->index;
    }
    else
    {

        if ((pHeader->index-ilast) >= 100) //序号非法
        {
            printf("index is not illegal :%d", pHeader->index);
            return;
        }

        if ((pHeader->index-ilast) >= 2) //序号不连续
        {
            printf("index is not continuous :%d", pHeader->index);
            pcap_breakloop(adhandle);
            return;
        }


        ilast = pHeader->index;
    }

    for (int i=0; i<pHeader->ilength; i++)
    {
        *pg_ds << pData[i];
    }

    if (pHeader->chEndPackage == 1)
    {
        //保存文件
        pg_file->close();
        delete pg_file;
        delete pg_ds;
        pg_file = nullptr;
        pg_ds = nullptr;
        printf("file save ok!");
    }

}
