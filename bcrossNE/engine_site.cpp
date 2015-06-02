/* 
 * File:   engine_site.cpp
 * Author: demiin
 * 
 * Created on May 21, 2015, 1:38 PM
 */

#include <boost/thread/thread_time.hpp>
#include <QtCore/qtemporaryfile.h>

#include "engine_site.h"

engine_site::engine_site() {
}

engine_site::~engine_site() {
}

void engine_site::stop() {

}

void engine_site::working() {
    tcp::socket socket(io_service);
    tcp::resolver::query query("www." + _siteToSearch, "80");
    tcp::resolver resolver(io_service) ;
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    boost::asio::streambuf response;
    string newPath;
    
    makeconn(&socket, &endpoint_iterator);
    sendRequest_getByISBN(&socket, _ISBN_autorbook);
    _status = getStatus(&socket, &response);
    _headers = getHeaders(&socket, &response);
    for (string header: _headers) {
        if (header.find("Location:") != string::npos) {
            newPath = parsing.getFullPath(header);
        }
    }
    if (newPath == "" || _siteToSearch.find("ozon") != string::npos) {
        siteContent[_siteToSearch] = getContent(&socket, &response);
        emit finished_loadInitSiteContent(siteContent);
        newPath = newPath == "" ? getFullPath() : newPath;
    }
    
    sendRequest_goFullPage(&socket, newPath);

    siteContent[_siteToSearch] = getContent(&socket, &response);
    
    if (searchManyAnswers()) {
        newPath = getFullPath();
        sendRequest_goFullPage(&socket, newPath);    
        siteContent[_siteToSearch] = getContent(&socket, &response);        
    }
    getBookInfo();
    emit finished_gettingBookInfo(make_pair(_siteToSearch, returnBookInfo));
    
    
}

void engine_site::makeconn(tcp::socket* socket, tcp::resolver::iterator* endpoint_iterator) {
    boost::system::error_code error = boost::asio::error::host_not_found;
    tcp::resolver::iterator end;    
    
    while (error && (*endpoint_iterator) != end)
    {
        (*socket).close();
        (*socket).connect(*(*endpoint_iterator)++, error);
    }
    if (error)
        throw boost::system::system_error(error);
    
}

void engine_site::sendRequest_init(tcp::socket* socket) {
	boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream.clear();
        request_stream << "GET / HTTP/1.1\r\n"
                   << "Host: murmanlib.ru\r\n"
                   << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                   << "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n";
//                   << "Accept-Encoding: gzip, deflate\r\n";
        
        if (!_cookie.empty())
            request_stream << "Cookie: " << _cookie << _cookie_magic << "\r\n";
        
        request_stream << "Connection: keep-alive\r\n\r\n";
        boost::asio::streambuf::const_buffers_type bufs = request.data();
        //cout << string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size()) << endl;;
        boost::asio::write(*socket, request);
        _cookie = "";
}

void engine_site::sendRequest_getByISBN(tcp::socket* socket, string isbn) {
	boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream.clear();
        string request_format;
        if (_siteToSearch.find("ozon") != string::npos) {
            request_format = "?context=search&group=div_book&text=" + parsing.replaceAllSpace(isbn, "%20");
        }
        else if (_siteToSearch.find("litres") != string::npos) {
            request_format = "pages/biblio_search/?q=" + parsing.replaceAllSpace(isbn,"+") + "&search_types=0";
        }
        else if (_siteToSearch.find("livelib") != string::npos) {
            request_format = "find/" + isbn;
        }
        
        request_stream << "GET /" + request_format + " HTTP/1.1\r\n"
                   << "Host: www." << _siteToSearch << "\r\n"
                   << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                   << "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n";
//                   << "Accept-Encoding: gzip, deflate\r\n";
        
        if (!_cookie.empty())
            request_stream << "Cookie: " << _cookie << _cookie_magic << "\r\n";
        
        request_stream << "Connection: keep-alive\r\n\r\n";
        boost::asio::streambuf::const_buffers_type bufs = request.data();
        //cout << string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size()) << endl;;
        boost::asio::write(*socket, request);
        _cookie = "";
}

void engine_site::sendRequest_goFullPageResult(tcp::socket* socket, string newpath) {
	boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream.clear();
        string request_format;
        if (_siteToSearch.find("ozon") != string::npos) {
            request_format = "?context=search&group=div_book&text=";
        }
        else if (_siteToSearch.find("litres") != string::npos) {
            request_format = "pages/biblio_search/?q=";
        }
        else if (_siteToSearch.find("livelib") != string::npos) {
            request_format = newpath;
        }
        
        request_stream << "GET /" + request_format + " HTTP/1.1\r\n"
                   << "Host: www." << _siteToSearch << "\r\n"
                   << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                   << "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n";
//                   << "Accept-Encoding: gzip, deflate\r\n";
        
        if (_cookie != "")
            request_stream << "Cookie: " << _cookie << _cookie_magic << "\r\n";
        
        request_stream << "Connection: keep-alive\r\n\r\n";
        boost::asio::streambuf::const_buffers_type bufs = request.data();
        //cout << string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size()) << endl;;
        boost::asio::write(*socket, request);
        _cookie = "";
}

void engine_site::sendRequest_goFullPage(tcp::socket* socket, string newpath) {
	boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream.clear();
        request_stream << "GET /" + newpath + " HTTP/1.1\r\n"
                   << "Host: www." << _siteToSearch << "\r\n"
                   << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                   << "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n";
//                   << "Accept-Encoding: gzip, deflate\r\n";
        
        if (!_cookie.empty())
            request_stream << "Cookie: " << _cookie << _cookie_magic << "\r\n";
        
        request_stream << "Connection: keep-alive\r\n\r\n";
        boost::asio::streambuf::const_buffers_type bufs = request.data();
        //cout << string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size()) << endl;;
        boost::asio::write(*socket, request);
        _cookie = "";
}

unsigned int engine_site::getStatus(tcp::socket* socket, boost::asio::streambuf* response) {
    istream response_stream(response);
    boost::asio::read_until((*socket), (*response), "\r\n");

    string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    string status_message;
    getline(response_stream, status_message);    
    return status_code;
}

vector<string> engine_site::getHeaders(tcp::socket* socket, boost::asio::streambuf* response) {
    istream response_stream(response);
    vector<string> headers;
    boost::asio::read_until((*socket), (*response), "\r\n\r\n");
 
    string header;
    while (std::getline(response_stream, header) && header != "\r") {
        headers.push_back(header);
    }
    return headers;
}

vector<string> engine_site::getContent(tcp::socket* socket, boost::asio::streambuf* response) {
    istream response_stream(response);
    try {
        boost::asio::read_until((*socket), (*response), "</html>");
    }
    catch (boost::system::system_error error)
    {
        cout << "end of file" << endl;
    }
    
    //boost::asio::streambuf::const_buffers_type bufs = (*response).data();
    //return string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + (*response).size());
    vector<string> content;
    string header;
    while (std::getline(response_stream, header)) {
        content.push_back(header);
    }    
  return content  ;
}

string engine_site::getFullPath() {
    string toSearch;
    if (_siteToSearch.find("ozon") != string::npos) {
        for (string &line: siteContent[_siteToSearch]) {
            if (line.find("<span itemprop=\"name\">") != string::npos)  {
                toSearch = line;
                break;
            }
        }
    } else if (_siteToSearch.find("livelib") != string::npos) {
        for (string line: siteContent[_siteToSearch]) {
            if (line.find("<div class=\"title\">") != string::npos) {
                toSearch = line;
                break;
            }
        }
    } else if (_siteToSearch.find("litres") != string::npos) {
        
    }
    return parsing.getFullPath(toSearch);
}

void engine_site::getBookInfo() {
    for (string &content: siteContent[_siteToSearch]) {
        //cout << content << endl;
        
        // Обложка
        if (content.find("<a class=\"bookpage-cover\" href=\"javascript:void(0)\" style=\"cursor: default;\"><img src") != string::npos) {
            //cout << content << endl;
            //cout << parsing.getImg(content) << endl;
            
            returnBookInfo[_ISBN_autorbook].picAddr = parsing.getImg(content);
        } else
        if (content.find("<img onclick=\"gallery_open_one") != string::npos) {
            //cout << content << endl;
            //cout << parsing.getImg(content) << endl;
            returnBookInfo[_ISBN_autorbook].picAddr = parsing.getImg(content);
        } else
        if (content.find("PreviewBig") != string::npos) {
            //cout << content << endl;
            //cout << parsing.getImg(content) << endl;
            returnBookInfo[_ISBN_autorbook].picAddr = parsing.getImg(content);
        } else
        
        
        
        // Название книги (и в некоторых случаях автор))
        if (content.find("itemprop=\"name\"") != string::npos) {
            //cout << content << endl;
            //cout << parsing.getBook_name(content) << endl;
            //cout << parsing.getBook_autor(content) << endl;
            if (returnBookInfo[_ISBN_autorbook].bookname == "") {
                returnBookInfo[_ISBN_autorbook].bookname = parsing.getBook_name(content);
            }
            if (returnBookInfo[_ISBN_autorbook].autor == "") {
                returnBookInfo[_ISBN_autorbook].autor = parsing.getBook_autor(content);

            }
        } else
        if (content.find("prodName") != string::npos) {
#ifndef _WIN32            
            //cout << parsing.iconv_recode("cp1251", "utf-8", content) << endl;
            content = parsing.iconv_recode("cp1251", "utf-8", content);
#else
            QTextCodec* defaultTextCodec = QTextCodec::codecForName("WINDOWS-1251");
            QTextDecoder *decoder = new QTextDecoder(defaultTextCodec);
            content = decoder->toUnicode(content.c_str(), content.length()).toStdString();
#endif

            //cout << parsing.getBook_name(content) << endl;
            if (returnBookInfo[_ISBN_autorbook].bookname == "") {
                returnBookInfo[_ISBN_autorbook].bookname = parsing.getBook_name(content);
                cout  << returnBookInfo[_ISBN_autorbook].bookname << endl;
            }
        } else
        if (content.find("<p  itemprop=\"author\">") != string::npos) {
#ifndef _WIN32            
            //cout << parsing.iconv_recode("cp1251", "utf-8", content) << endl;
            content = parsing.iconv_recode("cp1251", "utf-8", content);
#else
            QTextCodec* defaultTextCodec = QTextCodec::codecForName("WINDOWS-1251");
            QTextDecoder *decoder = new QTextDecoder(defaultTextCodec);
            content = decoder->toUnicode(content.c_str(), content.length()).toStdString();
#endif

            //cout << parsing.getBook_autor(content) << endl;
            returnBookInfo[_ISBN_autorbook].autor = parsing.getBook_autor(content);
        } else
        
        
        // Описание книги
        if (content.find("og:description\" content=\"") != string::npos) {
            //cout << content << endl;
            //cout << parsing.getBook_descr(content) << endl;
            returnBookInfo[_ISBN_autorbook].descr = parsing.getBook_descr(content);
        } else
        if (content.find("itemprop=\"about\"") != string::npos) {
            //cout << content << endl;
            //cout << parsing.getBook_descr(content) << endl;
            returnBookInfo[_ISBN_autorbook].descr = parsing.getBook_descr(content);
        } else
        if (content.find("ANNOTATION") != string::npos) {
#ifndef _WIN32            
            //cout << parsing.iconv_recode("cp1251", "utf-8", content) << endl;
            content = parsing.iconv_recode("cp1251", "utf-8", content);
#else
            QTextCodec* defaultTextCodec = QTextCodec::codecForName("WINDOWS-1251");
            QTextDecoder *decoder = new QTextDecoder(defaultTextCodec);
            content = decoder->toUnicode(content.c_str(), content.length()).toStdString();
#endif


            //cout << parsing.getBook_descr(content) << endl;
            returnBookInfo[_ISBN_autorbook].descr = parsing.getBook_descr(content);
        }
    }
}

bool engine_site::searchManyAnswers() {
    for (string line: siteContent[_siteToSearch]) {
        if (line.find("searchresult") != string::npos) {
            return true;
        }
    }
    
    return false;
}

void engine_site::flushToSite() {
    tcp::socket socket(io_service);
    tcp::resolver::query query("murmanlib.ru", "80");
    tcp::resolver resolver(io_service) ;
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    boost::asio::streambuf response;

    makeconn(&socket, &endpoint_iterator);
    sendRequest_init(&socket);
    getStatus(&socket, &response);
    vector<string> headers = getHeaders(&socket, &response);
    for (string &header: headers) {
  //      cout << header << endl;
    }
    vector<string> content = getContent(&socket, &response);
    sendRequest_loginInit(&socket);
    do {
        headers = getHeaders(&socket, &response);
    
        for (string &header: headers) {
            if (header.find("SESS") != string::npos) {
                cookie = parsing.getCookie(header);
            }
        }    
    } while (cookie == "");
    
    sendRequest_getAddPage(&socket);
    getStatus(&socket, &response);
    headers = getHeaders(&socket, &response);
    
    content = getContent(&socket, &response); 
    vector<string> fID;
    vector<string> fT;
    
    for (string &line: content) {
        if (line.find("form_build_id") != string::npos) {
                //formID = parsing.getFormID(line);
            fID.push_back(parsing.getFormID(line));
        } 
        if (line.find("form_token") != string::npos) {
                //formToken = parsing.getFormToken(line);
                fT.push_back(parsing.getFormToken(line));
        }
    }    
    formID = fID.front();
    formToken = fT.front();
 
    makeFormBody();
    makeAddPage();
    sendRequest_AddPage(&socket);
    //content = getContent(&socket, &response); 
}

void engine_site::sendRequest_loginInit(tcp::socket* socket) {
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
    request_stream << "POST /node?destination=node HTTP/1.1\r\n"
                   << "Host: murmanlib.ru\r\n"
                   << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                   << "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
//                   << "Accept-Encoding: gzip, deflate\r\n"
                   << "Referer: http://murmanlib.ru/\r\n"
                   << "Cookie: " << cookie << cookie_magic << "\r\n"
                   << "Connection: keep-alive\r\n"
                   << "Content-Type: application/x-www-form-urlencoded\r\n"
                   << "Content-Length: " << login_site.length() << "\r\n\r\n"
                   << login_site;
    
    boost::asio::streambuf::const_buffers_type bufs = request.data();
//        cout << string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size()) << endl;;
    boost::asio::write(*socket, request);
    cookie = "";
}

void engine_site::sendRequest_getAddPage(tcp::socket* socket) {
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
    request_stream << "GET /node/add/page HTTP/1.1\r\n"
                   << "Host: murmanlib.ru\r\n"
                   << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                   << "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
//                   << "Accept-Encoding: gzip, deflate\r\n"
                   << "Referer: http://murmanlib.ru/node/add\r\n"
                   << "Cookie: " << cookie << "; " << cookie_magic << "\r\n"
                   << "Connection: keep-alive\r\n\r\n";
    
    boost::asio::streambuf::const_buffers_type bufs = request.data();
//        cout << string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size()) << endl;;
    boost::asio::write(*socket, request);
}

void engine_site::sendRequest_AddPage(tcp::socket* socket) {
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
    request_stream << "POST /node/add/page HTTP/1.1\r\n"
                   << "Host: murmanlib.ru\r\n"
                   << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                   << "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
//                   << "Accept-Encoding: gzip, deflate\r\n"
                   << "Referer: http://murmanlib.ru/node/add/page\r\n"
                   << "Cookie: " << cookie /*<< "; " << cookie_magic*/ << "\r\n"
                   << "Connection: keep-alive\r\n"
                   << "Content-Type: multipart/form-data; boundary=---------------------------15914381318027603441113782851\r\n"
                   << "Content-Length: " << sourceAddPage.length() << "\r\n\r\n"
                   << sourceAddPage;
    
    boost::asio::streambuf::const_buffers_type bufs = request.data();
//        cout << string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size()) << endl;;
    boost::asio::write(*socket, request);
    cookie = "";
}


void engine_site::makeFormBody() {
    formBody = 
               "<?php "
               "echo '<form action=\"modules/bcross_accept.php\" method=\"post\">';"
               "echo '<p style=\"text-align: justify;\">Предлагаем Вам заполнить следующие поля и помочь нам составить дневник путешествия Книги. <br />Bookcrossing  - общественное движение, действующее по принципу социальных сетей и близкое к флешмобу.  Человек, прочитав книгу, оставляет её в общественном месте (парк, кафе, поезд, станция метро), для того, чтобы другой, случайный человек мог эту книгу найти и прочитать; тот в свою очередь должен повторить эти же действия.</p>"
               "<table border=\"0\">"
               "<tbody>"
               "<tr>"
               "<td><img width=\"300\" height=\"300\" src=\"http://" + searchBook.picAddr + /*http://static2.ozone.ru/multimedia/books_covers/c300/1000890179.jpg*/ "\" border=\"0\" /></td>"
               "<td><p style=\"text-align: justify;\">" + searchBook.descr + /*Несправедливо осужденный трибуналом Воин Ветра, офицер воздушного флота Российской империи Егор Сморода поставлен перед выбором: сгнить на каторге или присоединиться к членам загадочного Института Прикладной Экзофизики, которые при помощи невероятного оружия очищают город от вампиров, демонов, оборотней и другой агрессивной нежити. Однако после того, как Сморода вступает в подпольную организацию &laquo;охотников за привидениями&raquo;, выясняется, что ставки в этой игре гораздо более высокие, чем ему казалось вначале. Впрочем, беглому каторжнику уже нечего терять.*/ + "</p></td>"
               "</tr>"
               "</tbody>"
               "</table>"
               "<p><strong>Ваше имя: </strong><br /> <input name=\"usrname\" size=\"50\" type=\"text\" /></p>"
               //"<p><strong>Возраст: </strong><select name=\"age\" size=\"1\"> <option>Возрастная категория</option> <option value=\"before14\">до 14 лет</option> <option value=\"after14\">15-24 года</option> <option value=\"after25\">25-50 лет</option> <option value=\"after50\">После 51 года</option> </select></p>"
               "<p><strong>Возраст: </strong><br /> <input name=\"age\" size=\"50\" type=\"text\" /></p>"
               "<p style=\"text-align: left;\"><strong>Где вы встретили Книгу:</strong><br /> <input name=\"geofind\" size=\"50\" type=\"text\" value=\"';"
               "include(\"modules/sypex_geo/SxGeo.php\");"
               "$SxGeo = new SxGeo('modules/sypex_geo/SxGeoCity.dat', SXGEO_FILE);"
               "$ip = $_SERVER['REMOTE_ADDR'];"
               "$city = $SxGeo->get($ip);"
               "print $city[\"city\"][\"name_ru\"]; "

                "$conn_str = \"host=192.168.9.250 port = 5432 dbname=bcross user=oa password=oa\";"
                "$conn = pg_connect($conn_str)"
                    "or die(\"error connect\". pg_last_error($conn));"

                    "$query = \"INSERT INTO readers VALUES ((SELECT COUNT (*) + 3 from readers), 'automatic', '0', '\". $city[\"city\"][\"name_ru\"]. \"',' ', '" + rancode + "', '\". date('d.m.Y'). \"');\";"
        
                "$res = pg_query($query)"
                    "or die (\"Error query: \". pg_last_error($conn));"
        
               "echo '\" />"
               "<p style=\"text-align: left;\"><strong>Где вы планируете ее оставить или передать другому читателю:</strong><br /> <input name=\"geoleave\" size=\"50\" type=\"text\" />"
               "<br /><br /><button>Отправить</button></p>"
               "<p style=\"text-align: right;\"><small>Обложки книг взяты с интернет-магазина <a href=\"http://www.ozon.ru\" target=\"_blank\"><small>www.ozon.ru</small></a></small></p>'; "
               "echo '<input name=\"book\" hidden=\"true\" value=\"" + rancode +"\">';"
               "echo '</form>';"
               "?>";
    
}

void engine_site::makeAddPage() {
    sourceAddPage = "-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"title\"\r\n\r\nВы попали на эту страницу, участвуя в акции \"Покажи Книге Мир\"\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"taxonomy_vocabulary_12[und]\"\r\n\r\n_none\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"taxonomy_vocabulary_13[und]]\"\r\n\r\n_none\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[link_title]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"changed\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"body[und][0][summary]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][data-inner-width]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][data-inner-height]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][title]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][item_attributes][data-inner-width]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][item_attributes][data-inner-height]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][item_attributes][id]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][item_attributes][class]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][item_attributes][style]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[parent]\"\r\n\r\nprimary-links:0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[weight]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][id]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][name]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][target]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][rel]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][class]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][style]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][accesskey]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"body[und][0][value]\"\r\n\r\n" + formBody + "\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"body[und][0][format]\"\r\n\r\n3\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"changed\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"form_build_id\"\r\n\r\n" + formID + "\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"form_token\"\r\n\r\n" + formToken + "\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"form_id\"\r\n\r\npage_node_form\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"log\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"files[field_image200150_und_0]\"; filename=\"\"\r\nContent-Type: application/octet-stream\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_image200150[und][0][_weight]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_image200150[und][0][fid]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_image200150[und][0][display]\"\r\n\r\n1\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"files[field_image200150_und_0]\"; filename=\"\"\r\nContent-Type: application/octet-stream\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_image150200[und][0][_weight]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_image150200[und][0][fid]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_image150200[und][0][display]\"\r\n\r\n1\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"files[upload_und_0]\"; filename=\"\"\r\nContent-Type: application/octet-stream\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"upload[und][0][_weight]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"upload[und][0][fid]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"upload[und][0][display]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_yamaps[und][0][coords]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_yamaps[und][0][type]\"\r\n\r\nyandex#map\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_yamaps[und][0][placemarks]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_yamaps[und][0][lines]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_yamaps[und][0][polygons]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_yamaps[und][0][routes]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[link_title]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[parent]\"\r\n\r\nmain-menu:0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[weight]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"additional_settings__active_tab\"\r\n\r\nedit-menu\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"path[alias]\"\r\n\r\n" + rancode + "\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"comment\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"files[upload]\"; filename=\"\"\r\nContent-Type: application/octet-stream\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"xmlsitemap[status]\"\r\n\r\ndefault\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"xmlsitemap[priority]\"\r\n\r\ndefault\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"name\"\r\n\r\nDemiin\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"date\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"status\"\r\n\r\n1\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"path[pathauto]\"\r\n\r\n1\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"op\"\r\n\r\n..................\r\n-----------------------------15914381318027603441113782851--\r\n";
    
}
