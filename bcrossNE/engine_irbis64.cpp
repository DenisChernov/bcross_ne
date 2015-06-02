/* 
 * File:   engine_irbis64.cpp
 * Author: demiin
 * 
 * Created on May 21, 2015, 1:39 PM
 */

#include "engine_irbis64.h"

engine_irbis64::engine_irbis64() {
    reqCounter = 1;
     
}

engine_irbis64::~engine_irbis64() {
}

void engine_irbis64::login_unlogin(string codeAPM, string codeCommand) 
{
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
   
    string prepareRequest = codeCommand + '\n' + codeAPM + '\n' + codeCommand + '\n'
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
                            + irbis64_login + '\n' + irbis64_password;

    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
    request_stream << prepareRequest;
    
    sendRequest(&request);
}


string engine_irbis64::sendRequest(boost::asio::streambuf* request) 
{
    tcp::resolver resolver(io_service) ;
    tcp::resolver::query query(server64, "6666");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;
    tcp::socket socket(io_service);
        
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    
    if (error)
        throw boost::system::system_error(error);
    
//    cout << "sended: " << boost::asio::write(socket, *request) << " bytes" << endl;
    boost::asio::write(socket, *request);
    /*******************************************/
    boost::asio::streambuf response;
    
    istream response_stream(&response);
    error = boost::asio::error::host_not_found;
    
    while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
    {
        //cout << &response;
    }
        
    if (error != boost::asio::error::eof)
        throw boost::system::system_error(error);
        
    boost::asio::streambuf::const_buffers_type bufs = response.data();
    
    socket.close();
    io_service.stop();

    return string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + response.size());
}

void engine_irbis64::sendRequest_noAnswer(boost::asio::streambuf* request) 
{
    tcp::resolver resolver(io_service) ;
    tcp::resolver::query query(server64, "6666");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;
    tcp::socket socket(io_service);
        
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    
    if (error)
        throw boost::system::system_error(error);
    
//    cout << "sended: " << boost::asio::write(socket, *request) << " bytes" << endl;
    boost::asio::write(socket, *request);
    /*******************************************/
    socket.close();
    io_service.stop();
}

void engine_irbis64::createBookRecord() {
    login_unlogin("C", "A");
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    generateQRcodes(rancode);
    string record =  string("0#0") + "\x1F" + string("0#0") + "\x1F" + "1#" + bookRecord.bookname + "\x1F" + 
                                                                                                   "2#" + bookRecord.autor + "\x1F" + 
                                                                                                   "3#" + bookRecord.ISBN_Autor + "\x1F" + 
                                                                                                   "7#^aФ 6" + "\x1F" + 
                                                                                                   string("10#\\\\192.168.6.8\\FileServerFolder\\qrcodes\\") + rancode + ".png" + "\x1F" + 
                                                                                                   string("11#http://murmanlib.ru/") + rancode + "\x1F";
    
    string prepareRequest = "D\nC\nD\n"
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
                            + irbis64_BCROSS_BASE + "\n0\n1\n" + record;

    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
    
    request_stream << prepareRequest;
    cout << prepareRequest << endl;
//    parsing parse(sendRequest(&request));
    cout << sendRequest(&request) << endl;
    
    login_unlogin("C", "B");
}

void engine_irbis64::getMaxMFN() 
{
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
   
    string prepareRequest = "O\nC\nO\n"
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
                            + irbis64_BCROSS_BASE;

    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
    request_stream << prepareRequest;
    maxMFN_BCROSS = parsing.maxMFN(sendRequest(&request));
    
}

void engine_irbis64::working() {
    vector<string> qrcode = genPageName(1000);
    
    for(QWidget* &page: (*listPages)) { // для всех найденных ISBN или связям - автор_название книги
        QList<QRadioButton*> rbs = page->findChildren<QRadioButton*>();
        for (QRadioButton* &rb: rbs) {
            engine_site* engine_SITE = new engine_site();
            if (rb->isChecked()) {
            string magiccode = genMagicCode();
                if (rb->objectName() == "ozon") {

                    vector<pair<string, map<string, searchResult>>>::const_iterator it = (*bookVariants).begin();
                    while (it != (*bookVariants).end()) {
                        bool done = false;
                        if (it->first.find("ozon") != string::npos) {
                            map<string, searchResult>::const_iterator it2 = it->second.begin();
                            
                            while (it2 != it->second.end()) {
                                if (it2->first.find(page->windowTitle().toStdString()) != string::npos) {
                                    this->setBookSearch(it2->second);
                                    qrcode.pop_back();
                                    this->setRancode(qrcode.back());
                                    this->setMAGIC_CODE(magiccode);
                                    engine_SITE->setSearchBook(it2->second);
                                    engine_SITE->setRancode(qrcode.back());
                                    done = true;
                                    break;
                                }
                                ++it2;
                            }
                            if (done) {
                                break;
                            }
                        }
                        ++it;
                    }
                    
                } else if (rb->objectName() == "livelib") {
                    vector<pair<string, map<string, searchResult>>>::const_iterator it = (*bookVariants).begin();
                    while (it != (*bookVariants).end()) {
                        bool done = false;
                        if (it->first.find("livelib") != string::npos) {
                            map<string, searchResult>::const_iterator it2 = it->second.begin();
                            
                            while (it2 != it->second.end()) {
                                if (it2->first.find(page->windowTitle().toStdString()) != string::npos) {
                                    this->setBookSearch(it2->second);
                                    qrcode.pop_back();
                                    this->setRancode(qrcode.back());

                                    this->setMAGIC_CODE(magiccode);
                                    engine_SITE->setSearchBook(it2->second);
                                    engine_SITE->setRancode(qrcode.back());
                                    done = true;
                                    break;
                                }
                                ++it2;
                            }
                            if (done) {
                                break;
                            }
                        }
                        ++it;
                    }
                    
                } else if (rb->objectName() == "litres") {
                    vector<pair<string, map<string, searchResult>>>::const_iterator it = (*bookVariants).begin();
                    while (it != (*bookVariants).end()) {
                        bool done = false;
                        if (it->first.find("litres") != string::npos) {
                            map<string, searchResult>::const_iterator it2 = it->second.begin();
                            
                            while (it2 != it->second.end()) {
                                if (it2->first.find(page->windowTitle().toStdString()) != string::npos) {
                                    this->setBookSearch(it2->second);
                                    qrcode.pop_back();
                                    this->setRancode(qrcode.back());
                                    this->setMAGIC_CODE(magiccode);
                                    engine_SITE->setSearchBook(it2->second);
                                    engine_SITE->setRancode(qrcode.back());
                                    done = true;
                                    break;
                                }
                                ++it2;
                            }
                            if (done) {
                                break;
                            }
                        }
                        ++it;
                    }
                    
                }
                //engine_IRBIS64->working();
                //engine_SITE->flushToSite();
                createBookRecord();
                engine_SITE->flushToSite();
//                thread_site->start();
                emit statusChanged(page->windowTitle() + QString(" добавлена"));
            }
        }
        
                                    
         
    }
    
}

void engine_irbis64::stop() {

}

void engine_irbis64::generateQRcodes(string pagename) 
{
#ifdef _WIN32
        //string step1 = "./createQR.sh http://murmanlib.ru/" + pagename + " qrTest.png \\192.168.6.8\\FileServerFolder\\qrcodes\\" + pagename;
	QDir dir;
	string step1 = dir.currentPath().toStdString() + "\\qrcode.exe -s 11 -l H \"http://murmanlib.ru/" + pagename + "\" -o " + dir.currentPath().toStdString() + "\\qrTest.png";
	string step2 = dir.currentPath().toStdString() + "\\convert.exe -size 450x450 xc:skyblue qrTest.png -geometry +0+0 -composite Logo_white.png -geometry +154+194 -composite \\\\192.168.6.8\\FileServerFolder\\qrcodes\\" + pagename + ".png";

        cout << step1 << endl;
        system(step1.c_str());
        cout << step2 << endl;
        system(step2.c_str());
    
#else
        string s = "./createQR.sh http://murmanlib.ru/" + pagename + " qrTest.png /mnt/filial6_.8_D/FileServerFolder/qrcodes/" + pagename;
#endif
        cout << s<< endl;
        system(s.c_str());
}


vector<string> engine_irbis64::genPageName(int count) 
{
    vector<string> tmp;
    
    std::mt19937 gen(time(0));
    std::uniform_int_distribution<> uid(10000000, 99999999);
    std::uniform_int_distribution<> simb(65, 90);
    std::uniform_int_distribution<> pos(0, 7);
    for (int page = 0; page < count; page++)
    {
        string s = boost::lexical_cast<string>(uid(gen));
        for (int i = 0; i <= 8; i++)
        {
            string::iterator it = s.begin();
            for (int position = 0; position < pos(gen); position++)
            {
                it++;
            }
            s.insert(it, char(simb(gen)));
        }
        tmp.push_back(s);
    }
    
    return tmp;
}

string engine_irbis64::genMagicCode() {
    std::mt19937 gen(time(0));
    std::uniform_int_distribution<> uid(100000, 999999);
    std::uniform_int_distribution<> simb(48, 57);
    std::uniform_int_distribution<> pos(0, 5);
        
    return boost::lexical_cast<string>(uid(gen));
}