/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainwindow
{
public:
    QWidget *centralwidget;
    QGroupBox *gbBookList;
    QGroupBox *gbISBN;
    QTextEdit *teISBN;
    QLabel *label;
    QLabel *lblBooksRemain;
    QGroupBox *gbResultSearch;
    QStackedWidget *swResultPages;
    QWidget *page;
    QLabel *lblResultFor;
    QComboBox *cbSearchInfo;
    QPushButton *pbSendToIRBIS64;
    QGroupBox *gbControl;
    QGroupBox *gbSwitchWherePars;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QPushButton *pbStart;
    QGroupBox *groupBox;
    QTextEdit *teStatus;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mainwindow)
    {
        if (mainwindow->objectName().isEmpty())
            mainwindow->setObjectName(QStringLiteral("mainwindow"));
        mainwindow->resize(1346, 902);
        centralwidget = new QWidget(mainwindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gbBookList = new QGroupBox(centralwidget);
        gbBookList->setObjectName(QStringLiteral("gbBookList"));
        gbBookList->setGeometry(QRect(10, 120, 481, 671));
        gbISBN = new QGroupBox(gbBookList);
        gbISBN->setObjectName(QStringLiteral("gbISBN"));
        gbISBN->setGeometry(QRect(10, 30, 461, 701));
        teISBN = new QTextEdit(gbISBN);
        teISBN->setObjectName(QStringLiteral("teISBN"));
        teISBN->setGeometry(QRect(10, 30, 441, 601));
        label = new QLabel(gbBookList);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(120, 0, 121, 16));
        lblBooksRemain = new QLabel(gbBookList);
        lblBooksRemain->setObjectName(QStringLiteral("lblBooksRemain"));
        lblBooksRemain->setGeometry(QRect(240, 0, 71, 16));
        gbResultSearch = new QGroupBox(centralwidget);
        gbResultSearch->setObjectName(QStringLiteral("gbResultSearch"));
        gbResultSearch->setGeometry(QRect(520, 0, 811, 791));
        swResultPages = new QStackedWidget(gbResultSearch);
        swResultPages->setObjectName(QStringLiteral("swResultPages"));
        swResultPages->setGeometry(QRect(10, 30, 791, 751));
        swResultPages->setFrameShape(QFrame::NoFrame);
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        swResultPages->addWidget(page);
        lblResultFor = new QLabel(gbResultSearch);
        lblResultFor->setObjectName(QStringLiteral("lblResultFor"));
        lblResultFor->setGeometry(QRect(220, 40, 111, 16));
        cbSearchInfo = new QComboBox(gbResultSearch);
        cbSearchInfo->setObjectName(QStringLiteral("cbSearchInfo"));
        cbSearchInfo->setGeometry(QRect(340, 36, 221, 23));
        cbSearchInfo->setEditable(true);
        pbSendToIRBIS64 = new QPushButton(gbResultSearch);
        pbSendToIRBIS64->setObjectName(QStringLiteral("pbSendToIRBIS64"));
        pbSendToIRBIS64->setGeometry(QRect(20, 36, 151, 23));
        pbSendToIRBIS64->setStyleSheet(QLatin1String("QPushButton {\n"
"    border: 2px solid #8f8f91;\n"
"    border-radius: 6px;\n"
"    background-color: rgb(238, 238, 238);\n"
"    min-width: 80px;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
"}\n"
""));
        gbControl = new QGroupBox(centralwidget);
        gbControl->setObjectName(QStringLiteral("gbControl"));
        gbControl->setGeometry(QRect(10, 0, 481, 101));
        gbSwitchWherePars = new QGroupBox(gbControl);
        gbSwitchWherePars->setObjectName(QStringLiteral("gbSwitchWherePars"));
        gbSwitchWherePars->setGeometry(QRect(10, 29, 461, 41));
        checkBox = new QCheckBox(gbSwitchWherePars);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(10, 20, 88, 21));
        checkBox->setChecked(true);
        checkBox_2 = new QCheckBox(gbSwitchWherePars);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(140, 20, 88, 21));
        checkBox_2->setChecked(false);
        checkBox_3 = new QCheckBox(gbSwitchWherePars);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setGeometry(QRect(290, 20, 88, 21));
        checkBox_3->setChecked(true);
        pbStart = new QPushButton(centralwidget);
        pbStart->setObjectName(QStringLiteral("pbStart"));
        pbStart->setGeometry(QRect(370, 80, 111, 41));
        pbStart->setStyleSheet(QLatin1String("QPushButton {\n"
"    border: 2px solid #8f8f91;\n"
"    border-radius: 6px;\n"
"    background-color: rgb(238, 238, 238);\n"
"    min-width: 80px;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
"}\n"
"\n"
"QPushButton:flat {\n"
"    border: none; /* no border for a flat push button */\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 790, 1321, 71));
        teStatus = new QTextEdit(groupBox);
        teStatus->setObjectName(QStringLiteral("teStatus"));
        teStatus->setGeometry(QRect(10, 30, 1301, 31));
        mainwindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mainwindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1346, 20));
        mainwindow->setMenuBar(menubar);
        statusbar = new QStatusBar(mainwindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        mainwindow->setStatusBar(statusbar);

        retranslateUi(mainwindow);

        swResultPages->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(mainwindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainwindow)
    {
        mainwindow->setWindowTitle(QApplication::translate("mainwindow", "\320\220\320\240\320\234 \320\221\321\203\320\272\320\272\321\200\320\276\321\201\321\201\320\270\320\275\320\263", 0));
        gbBookList->setTitle(QApplication::translate("mainwindow", "\320\241\320\277\320\270\321\201\320\272\320\270 \320\272\320\275\320\270\320\263", 0));
        gbISBN->setTitle(QApplication::translate("mainwindow", "\320\244\320\276\321\200\320\274\320\260\321\202: \"ISBN\" \320\270\320\273\320\270 \"\320\220\320\262\321\202\320\276\321\200 \320\267\320\260\320\263\320\273\320\260\320\262\320\270\320\265\", \320\261\320\265\320\267 \320\272\320\260\320\262\321\213\321\207\320\265\320\272", 0));
        label->setText(QApplication::translate("mainwindow", "\320\232\320\275\320\270\320\263 \320\264\320\273\321\217 \320\277\320\276\320\270\321\201\320\272\320\260:", 0));
        lblBooksRemain->setText(QApplication::translate("mainwindow", "0", 0));
        gbResultSearch->setTitle(QApplication::translate("mainwindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202 \320\277\320\276\320\270\321\201\320\272\320\260", 0));
        lblResultFor->setText(QApplication::translate("mainwindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202 \320\264\320\273\321\217:", 0));
        pbSendToIRBIS64->setText(QApplication::translate("mainwindow", "\320\236\321\202\320\277\321\200\320\260\320\262\320\270\321\202\321\214 \320\262 IRBIS64", 0));
        gbControl->setTitle(QApplication::translate("mainwindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", 0));
        gbSwitchWherePars->setTitle(QApplication::translate("mainwindow", "\320\241\320\260\320\271\321\202\321\213 \320\264\320\273\321\217 \320\277\320\260\321\200\321\201\320\270\320\275\320\263\320\260", 0));
        checkBox->setText(QApplication::translate("mainwindow", "ozon.ru", 0));
        checkBox_2->setText(QApplication::translate("mainwindow", "livelib.ru", 0));
        checkBox_3->setText(QApplication::translate("mainwindow", "litres.ru", 0));
        pbStart->setText(QApplication::translate("mainwindow", "\320\237\320\276\320\273\321\203\321\207\320\270\321\202\321\214", 0));
        groupBox->setTitle(QApplication::translate("mainwindow", "\320\241\321\202\320\260\321\202\321\203\321\201 \320\262\321\213\320\277\320\276\320\273\320\275\320\265\320\275\320\270\321\217", 0));
    } // retranslateUi

};

namespace Ui {
    class mainwindow: public Ui_mainwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
