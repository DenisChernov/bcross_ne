/* 
 * File:   engine_irbis64.h
 * Author: demiin
 *
 * Created on May 21, 2015, 1:39 PM
 */

#ifndef ENGINE_IRBIS64_H
#define	ENGINE_IRBIS64_H

#include <cstdlib>
#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <boost/system/system_error.hpp>
#include <map>
#include <boost/date_time.hpp>
#include <vector>
#include <ctime>
#include <QThread>
#include <QRadioButton>
#ifdef _WIN32
    #include <random>
#endif
#include "parser.h"
#include "engine_site.h"

using namespace std;
using boost::asio::ip::tcp;

struct bookinfo
{
    string autor;
    string bookname;
    string annotation;
    string coverPath;
    string qrcode;
    string pagename;
};

class engine_irbis64 : public QObject{
    Q_OBJECT
public:
    engine_irbis64();
    virtual ~engine_irbis64();
    void setBookSearch(searchResult bookSearch) {
        this->bookRecord = bookSearch;
    }
    
    void setISBN(string ISBN) {
        this->ISBN = ISBN;
    }

    void setRancode(string rancode) {
        this->rancode = rancode;
    }

    void setMAGIC_CODE(string MAGIC_CODE) {
        this->MAGIC_CODE = MAGIC_CODE;
    }
    
    void setBookVariants(vector<pair<string, map<string, searchResult> > >* bookVariants) {
        this->bookVariants = bookVariants;
    }

    void setListPages(QList<QWidget*>* listPages) {
        this->listPages = listPages;
    }
    
private:
    QList<QWidget*> *listPages;
    vector<pair<string, map<string, searchResult>>>* bookVariants;
    
    string maxMFN_BCROSS = "1";
    string MAGIC_CODE = "31771";
    size_t reqCounter;
    boost::asio::io_service io_service;
    map <string, string> bookStorage;
    map <string, string> responseTypes;
    string currentISBN = "";
    string irbis64_login    = "irbisoft";
    string irbis64_password = "9f9@7Nuq";
    string irbis64_countAnswers = "1000";
    string server64         = "192.168.9.249";
    string irbis64_BCROSS_BASE = "BCROSS";
    parser parsing;
    searchResult bookRecord;
    string ISBN;
    string rancode;
    
    void login_unlogin(string codeAPM, string codeCommand);
    string sendRequest(boost::asio::streambuf* request);
    void sendRequest_noAnswer(boost::asio::streambuf* request);
    void createBookRecord();
    void getMaxMFN();
    void generateQRcodes(string pagename);
    string genMagicCode();
    vector<string> genPageName(int count) ;
    
signals:
    void finished();
    void statusChanged(QString);
    
public slots:
    void working();
    void stop();
    
};

#endif	/* ENGINE_IRBIS64_H */

