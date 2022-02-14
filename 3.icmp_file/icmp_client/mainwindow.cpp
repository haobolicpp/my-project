#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <thread>
#include <QThread>

#define PACK_SIZE 1024

#pragma pack(1)
typedef struct TFileHeader
{
    char chEndPackage;
    int index;
    int ilength;
}TFileHeader;
#pragma pack()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_ctrl.Init();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_selectFile_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this);
    ui->edt_file->setText(strFileName);

    //读取文件
    QFile file(strFileName);
    file.open(QFile::ReadOnly);
    m_readBt = file.readAll();
    m_ipack_total = m_readBt.length() / (PACK_SIZE-sizeof (TFileHeader));
    if (m_readBt.length() % (PACK_SIZE-sizeof (TFileHeader)) != 0)
    {
        m_ipack_total++;
    }

    ui->label_total->setNum(m_ipack_total);
}

void MainWindow::on_btn_send_clicked()
{
//    ctrl.SendData((char*)psend, strlen(psend), "10.60.66.16");
//    char buffer[1024*2] = {0};
//    int i = ctrl.RecvData(buffer, 1024*2);

    ui->label_sended->setEnabled(false);

    std::thread *psendth = new std::thread([&](){
        int index_package = 1;
        while (index_package <= m_ipack_total)
        {
            char buffer[PACK_SIZE] = {0};
            TFileHeader header;
            header.index = index_package;
            if (index_package == m_ipack_total)
            {
                header.ilength = m_readBt.length() % (PACK_SIZE-sizeof (TFileHeader));
                header.chEndPackage = 1;
            }
            else
            {
                header.ilength = PACK_SIZE-sizeof (TFileHeader);
                header.chEndPackage = 0;
            }
            memcpy(buffer, &header, sizeof (TFileHeader));
            memcpy(buffer+sizeof (TFileHeader), m_readBt.data()+(index_package-1)*(PACK_SIZE-sizeof (TFileHeader)), header.ilength);


            m_ctrl.SendData((char*)buffer, sizeof (TFileHeader)+header.ilength, "10.60.66.16");

            index_package++;


            //等待接收应答
            char buffer_r[PACK_SIZE*2] = {0};
            if (m_ctrl.RecvData(buffer_r, 1024*2) < 0)
            {
                //重发一次
                m_ctrl.SendData((char*)buffer, sizeof (TFileHeader)+header.ilength, "10.60.66.16");
                m_ctrl.RecvData(buffer_r, 1024*2);
            }

            //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        ui->label_sended->setNum(index_package);
    });

    ui->label_sended->setEnabled(true);
}
