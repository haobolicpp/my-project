#include "mainwindow.h"

#include <QApplication>

#include "ICMPControl.h"
#include "JsonFileResolver.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();




    return a.exec();
}
