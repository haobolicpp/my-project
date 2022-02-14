/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btn_selectFile;
    QLineEdit *edt_file;
    QLabel *label;
    QLabel *label_total;
    QLabel *label_3;
    QLabel *label_sended;
    QLabel *label_5;
    QPushButton *btn_send;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        btn_selectFile = new QPushButton(centralwidget);
        btn_selectFile->setObjectName(QString::fromUtf8("btn_selectFile"));
        btn_selectFile->setGeometry(QRect(40, 60, 75, 23));
        edt_file = new QLineEdit(centralwidget);
        edt_file->setObjectName(QString::fromUtf8("edt_file"));
        edt_file->setGeometry(QRect(120, 60, 321, 20));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 120, 101, 16));
        label_total = new QLabel(centralwidget);
        label_total->setObjectName(QString::fromUtf8("label_total"));
        label_total->setGeometry(QRect(170, 120, 101, 16));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 150, 101, 16));
        label_sended = new QLabel(centralwidget);
        label_sended->setObjectName(QString::fromUtf8("label_sended"));
        label_sended->setGeometry(QRect(170, 150, 101, 16));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(50, 180, 101, 16));
        btn_send = new QPushButton(centralwidget);
        btn_send->setObjectName(QString::fromUtf8("btn_send"));
        btn_send->setGeometry(QRect(40, 90, 75, 23));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        btn_selectFile->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251", nullptr));
        label->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\345\214\205\344\270\252\346\225\260\357\274\232", nullptr));
        label_total->setText(QApplication::translate("MainWindow", "1", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "\345\267\262\345\217\221\351\200\201\344\270\252\346\225\260\357\274\232", nullptr));
        label_sended->setText(QApplication::translate("MainWindow", "1", nullptr));
        label_5->setText(QString());
        btn_send->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
