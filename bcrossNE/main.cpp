/*
 * File:   main.cpp
 * Author: demiin
 *
 * Created on May 13, 2015, 4:44 PM
 */

#include <QApplication>
#include "mainwindow.h"
#include <QDir>

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

#ifdef _WIN32
	QCoreApplication::addLibraryPath(".");
#endif    
    
    QApplication app(argc, argv);
    qRegisterMetaType<map<string, vector<string>> >("map<string, vector<string>>");
    qRegisterMetaType<map<string, searchResult> >("map<string, searchResult>");
    qRegisterMetaType<pair<string, map<string, searchResult>> >("pair<string, map<string, searchResult>>");
    std::ios::sync_with_stdio(false);
    
    mainwindow mainWnd;
    mainWnd.show();
    // create and show your widgets here

    
    return app.exec();
}
