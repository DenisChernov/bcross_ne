/* 
 * File:   engine_site.h
 * Author: demiin
 *
 * Created on May 21, 2015, 1:38 PM
 */

#ifndef ENGINE_SITE_H
#define	ENGINE_SITE_H

#include <QObject>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <map>
#include <QTextCodec>
#include "parser.h"

using namespace std;
using boost::asio::ip::tcp;



class engine_site : public QObject { 
    Q_OBJECT
public:
    engine_site();
    virtual ~engine_site();
    void setSiteToSearch(string _siteToSearch) {
        this->_siteToSearch = _siteToSearch;
    }
    
    string getSiteToSearch() const {
        return _siteToSearch;
    }
    
    void setISBN_autorbook(string _ISBN_autorbook) {
        this->_ISBN_autorbook = _ISBN_autorbook;
    }

    void setSearchBook(searchResult searchBook) {
        this->searchBook = searchBook;
    }

    void setRancode(string rancode) {
        this->rancode = rancode;
    }

private:
    boost::asio::io_service io_service;
    boost::asio::streambuf _request;        
    string _siteToSearch;
    string _cookie;
    string _cookie_magic;
    string _ISBN_autorbook;
    map<string, vector<string>> siteContent;
    vector<string> content;
    string _status;
    vector<string> _headers;
    parser parsing;
    map<string, searchResult> returnBookInfo;
    searchResult searchBook;
    string formID;
    string formToken;
    string login_site = "name=Demiin&pass=pr0tsa5h&form_id=user_login_block&op=%D0%92%D1%85%D0%BE%D0%B4+%D0%B2+%D1%81%D0%B8%D1%81%D1%82%D0%B5%D0%BC%D1%83&form_build_id=";
    string cookie_magic = "has_js=1; _ga=GA1.2.438614576.1426181815; _gat=1; _ym_visorc_27916745=w; _ym_visorc_27916755=w; _ym_visorc_27433225=w";
    string cookie = "";
    string formBody;
    string rancode;
    string sourceAddPage;
    
    void makeconn(tcp::socket* socket, tcp::resolver::iterator* endpoint_iterator);
    void sendRequest_init(tcp::socket* socket);
    void sendRequest_loginInit(tcp::socket* socket);
    void sendRequest_getAddPage(tcp::socket* socket);
    void sendRequest_AddPage(tcp::socket* socket);
    void sendRequest_getByISBN(tcp::socket* socket, string isbn);
    void sendRequest_goFullPageResult(tcp::socket* socket, string newpath);
    void sendRequest_goFullPage(tcp::socket* socket, string newpath);
    unsigned int getStatus(tcp::socket* socket, boost::asio::streambuf* response);
    vector<string> getHeaders(tcp::socket* socket, boost::asio::streambuf* response);
    vector<string> getContent(tcp::socket* socket, boost::asio::streambuf* response);
    string getFullPath();
    void getBookInfo();
    bool searchManyAnswers();
    void makeFormBody();
    void makeAddPage();
    
signals:    
    void finished();
    void finished_loadInitSiteContent(map<string, vector<string>> content);
    void finished_gettingBookInfo(pair<string, map<string, searchResult>> returnBookInfo);
    

public slots:
    void working();
    void flushToSite();
    void stop();
};

#endif	/* ENGINE_SITE_H */

