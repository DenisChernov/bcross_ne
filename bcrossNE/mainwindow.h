/* 
 * File:   mainwindow.h
 * Author: demiin
 *
 * Created on May 15, 2015, 10:02 AM
 */

#ifndef _MAINWINDOW_H
#define	_MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QThread>
#include <iostream>
#include <string>
#include <vector>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QtWebKitWidgets/QWebView>
//#include <QtWebEngineWidgets/QWebEnginePage>
//#include <QtWebEngineWidgets/QWebEngineView>
#ifdef _WIN32
    #include <random>
#endif
#include "engine_irbis64.h"
#include "engine_site.h"

using namespace std;

class mainwindow : public QMainWindow {
    Q_OBJECT
public:
    mainwindow();
    virtual ~mainwindow();
private:
    Ui::mainwindow frmMain;
    vector<string> listToSearch;
    parser parsing;
    QList<QWidget*> listPages;
    vector<pair<string, map<string, searchResult>>> bookVariants;
    
    vector<string> siteWhereSearch();
    void thread_finishedLoadInitSiteContent(map<string, vector<string>> content);
    void thread_finishedGettingBookInfo(pair<string, map<string, searchResult>> result);
    vector<string> loadISBNlist();
    vector<string> genPageName(int count);
    string genMagicCode();
    int count_for_magic = 1;

public slots:
    void slot_getStarted();
    void slot_cbCurrentSearchFilterChanged(int);
    void slot_pbSendToIRBIS64();
    void slot_calculateBooksCount();
    void slot_bookStatusChanged(QString);
    
signals:
    void stopAll();



};

#endif	/* _MAINWINDOW_H */
