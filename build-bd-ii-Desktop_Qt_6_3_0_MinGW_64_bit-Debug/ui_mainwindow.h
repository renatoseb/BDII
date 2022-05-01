/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *editTitle;
    QPushButton *searchBtn;
    QPushButton *rangeBtn;
    QPushButton *insertBtn;
    QPushButton *deleteBtn;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        editTitle = new QLineEdit(centralwidget);
        editTitle->setObjectName(QString::fromUtf8("editTitle"));
        editTitle->setGeometry(QRect(240, 120, 341, 21));
        searchBtn = new QPushButton(centralwidget);
        searchBtn->setObjectName(QString::fromUtf8("searchBtn"));
        searchBtn->setGeometry(QRect(50, 180, 101, 41));
        rangeBtn = new QPushButton(centralwidget);
        rangeBtn->setObjectName(QString::fromUtf8("rangeBtn"));
        rangeBtn->setGeometry(QRect(260, 180, 101, 41));
        insertBtn = new QPushButton(centralwidget);
        insertBtn->setObjectName(QString::fromUtf8("insertBtn"));
        insertBtn->setGeometry(QRect(470, 180, 91, 41));
        deleteBtn = new QPushButton(centralwidget);
        deleteBtn->setObjectName(QString::fromUtf8("deleteBtn"));
        deleteBtn->setGeometry(QRect(660, 180, 81, 41));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        searchBtn->setText(QCoreApplication::translate("MainWindow", "Search", nullptr));
        rangeBtn->setText(QCoreApplication::translate("MainWindow", "Range Search", nullptr));
        insertBtn->setText(QCoreApplication::translate("MainWindow", "Insert", nullptr));
        deleteBtn->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
