#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ICMPControl.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_selectFile_clicked();

    void on_btn_send_clicked();

private:
    Ui::MainWindow *ui;

    CICMPControl m_ctrl;
    QByteArray m_readBt;
    int m_ipack_total;
};
#endif // MAINWINDOW_H
