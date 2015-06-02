/*
 * File:   mainwindow.cpp
 * Author: demiin
 *
 * Created on May 15, 2015, 10:02 AM
 */

#include "mainwindow.h"

mainwindow::mainwindow() {
    frmMain.setupUi(this);
    frmMain.teISBN->insertPlainText("5-05-005672-1\n");

    
        
    connect(frmMain.pbStart, SIGNAL(clicked()), this, SLOT(slot_getStarted()));
    connect(frmMain.cbSearchInfo, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_cbCurrentSearchFilterChanged(int)));
    connect(frmMain.pbSendToIRBIS64, SIGNAL(clicked()), this, SLOT(slot_pbSendToIRBIS64()));
    connect(frmMain.teISBN, SIGNAL(textChanged()), this, SLOT(slot_calculateBooksCount()));
}

mainwindow::~mainwindow() {
}

vector<string> mainwindow::siteWhereSearch() {
    vector<string> site;
    QList<QCheckBox* > listCheckBox = frmMain.gbSwitchWherePars->findChildren<QCheckBox *>();
    for (QCheckBox* &list: listCheckBox) {
        if (list->isChecked()) {
            site.push_back(list->text().toStdString());
        }
    }
    return site;
}

void mainwindow::slot_getStarted() {
    vector<string> siteList = siteWhereSearch();
    if (frmMain.teISBN->toPlainText().length() != 0)  {
        frmMain.cbSearchInfo->clear();
        for (QWidget* &widget: listPages) {
            frmMain.swResultPages->removeWidget(widget);
        }
        listPages.clear();
        listToSearch = loadISBNlist();
    }
    
    for (string &filter: listToSearch) {
        QWidget* newPageWidget = new QWidget;
        newPageWidget->setWindowTitle(QString::fromStdString(filter));
//        cout << newPageWidget->windowTitle().toStdString() << endl;
        listPages.append(newPageWidget);
        frmMain.swResultPages->addWidget(newPageWidget);
        for (string &site: siteList) {

//            cout << "thread start for: " << site << " with: "<< filter<< endl;
            engine_site* engine = new engine_site();
            QThread* thread = new QThread;
            engine->moveToThread(thread);
            engine->setSiteToSearch(site);

            engine->setISBN_autorbook(filter);
            connect(thread, &QThread::started, engine, &engine_site::working);
            connect(engine, &engine_site::finished, thread, &QThread::quit);
            connect(engine, &engine_site::finished_loadInitSiteContent, this, &mainwindow::thread_finishedLoadInitSiteContent);
            connect(engine, &engine_site::finished_gettingBookInfo, this, &mainwindow::thread_finishedGettingBookInfo);
            thread->start();
        }
    }
}

void mainwindow::thread_finishedLoadInitSiteContent(map<string, vector<string>> content) {
    map<string, vector<string>>::const_iterator it = content.begin();
//    cout << "load finished:" << it->first << "\t\t with lines: " << it->second.size() << endl;
}

void mainwindow::thread_finishedGettingBookInfo(pair<string, map<string, searchResult>> result) {
//    cout << "finished: " << result.first << endl;
    if (result.second.size() != 0) {
        map<string, searchResult>::iterator it = result.second.begin();
        cout << result.first << ": " << it->first << endl;
        it->second.ISBN_Autor = parsing.remove_all(it->first, "\n");
        bookVariants.push_back(result);
        
        if (it->second.picAddr.find("http://") != string::npos) {
            it->second.picAddr = it->second.picAddr.replace(0, 7, "");
            //cout << it->second.picAddr << endl;
        }
        //QWebEngineView* view = new QWebEngineView;
        QWebView* view = new QWebView;;
        QString htmlPage = 
            "<html>"
            "<body>"
            "<table border=\"0\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;\" cellspacing=\"2\" cellpadding=\"0\">"
            "<tr>"
            "<td valign=\"top\"><img height=\"210\" src=\"http://" + QString::fromStdString(it->second.picAddr) + "\"/></td>"
            "<td>"
            "<table border=\"0\">"
            "<tr><td><b>" + QString::fromStdString(it->second.autor) + "</b></td> </tr>"
            "<tr><td><b>" + QString::fromStdString(it->second.bookname) + "</b></td></tr>"
            "<tr><td>" + QString::fromStdString(it->second.descr) + "</td></tr>"
            "</table>"
            "</td>"
            "</tr>"
            "</table>"
            "</body>"
            "</html>";
        view->setHtml(htmlPage);
        view->setGeometry(0, 0, 751, 231);
        
 
        
        for(QWidget* &widget: listPages) {
            if (widget->windowTitle().toStdString().find(it->first) != string::npos) {
//                cout << "create view on: " << widget->windowTitle().toStdString() << endl;
                widget->setEnabled(true);
                //view = new QWebEngineView(widget);
                view->setParent(widget);
                QRadioButton* rb_1 = new QRadioButton;
                rb_1->setParent(widget);
                rb_1->move(6, 180);
                rb_1->setObjectName("ozon");
                rb_1->show();
                QRadioButton* rb_2 = new QRadioButton;
                rb_2->setParent(widget);
                rb_2->move(6, 410);
                rb_2->setObjectName("livelib");
                rb_2->show();
                QRadioButton* rb_3 = new QRadioButton;
                rb_3->setParent(widget);
                rb_3->move(6, 620);
                rb_3->setObjectName("litres");
                rb_3->show();
                
                if (result.first.find("ozon") != string::npos) {
                    view->move(40, 70);
                    rb_1->setChecked(true);
                } else
                if (result.first.find("livelib") != string::npos) {
                    view->move(40, 310);
                    rb_2->setChecked(true);
                } else
                if (result.first.find("litres") != string::npos) {
                    view->move(40, 530);
                    rb_3->setChecked(true);
                }
                view->show();
                break;
            }
        }
        //cout << " with: " << it->second.ISBN_Autor << endl;
        
                

        
    }        
    
    if (frmMain.swResultPages->currentIndex() != 1) {
        frmMain.swResultPages->setCurrentIndex(1);
    }
}

vector<string> mainwindow::loadISBNlist() {
    vector<string> list;
    if (frmMain.teISBN->toPlainText().toStdString().find('\n') !=  string::npos) {
        size_t pos = 0;
        
        do  {
            string s = frmMain.teISBN->toPlainText().toStdString().substr(pos, frmMain.teISBN->toPlainText().toStdString().find('\n', pos + 1) - pos);
            if (s != "\n") {
                list.push_back(s);
            }
            pos = frmMain.teISBN->toPlainText().toStdString().find('\n', pos + 1);
        }  while (pos !=  string::npos);

        for(string &s: list) {
            if (s.find('\n') != string::npos) {
                s = s.replace(s.find('\n'), 1, "");
            }
            if (s != "") {
                frmMain.cbSearchInfo->addItem(QString::fromStdString(s));
            }
        }
    }
    else {
        frmMain.cbSearchInfo->addItem(frmMain.teISBN->toPlainText());
        list.push_back(frmMain.teISBN->toPlainText().toStdString());
    }
    
    frmMain.cbSearchInfo->setCurrentIndex(0);
    
    return list;
}

void mainwindow::slot_cbCurrentSearchFilterChanged(int page) {
    frmMain.swResultPages->setCurrentIndex(page + 1);
}

void mainwindow::slot_pbSendToIRBIS64() {
    
    engine_irbis64* engine_IRBIS64 = new engine_irbis64();
    QThread* thread_irbis64 = new QThread;
    engine_IRBIS64->moveToThread(thread_irbis64);
    engine_IRBIS64->setListPages(&listPages);
    engine_IRBIS64->setBookVariants(&bookVariants);
    connect(thread_irbis64, &QThread::started, engine_IRBIS64, &engine_irbis64::working);
    connect(engine_IRBIS64, &engine_irbis64::finished, thread_irbis64, &QThread::quit);
    connect(engine_IRBIS64, &engine_irbis64::statusChanged, this, &mainwindow::slot_bookStatusChanged);
    thread_irbis64->start();
/*    
    for(QWidget* &page: listPages) { // для всех найденных ISBN или связям - автор_название книги
        QList<QRadioButton*> rbs = page->findChildren<QRadioButton*>();
        for (QRadioButton* &rb: rbs) {
            string magiccode = genMagicCode();
            if (rb->isChecked()) {
                engine_irbis64* engine_IRBIS64 = new engine_irbis64();
                engine_site* engine_SITE = new engine_site();
                QThread* thread_irbis64 = new QThread;
                QThread* thread_site = new QThread;
                engine_IRBIS64->moveToThread(thread_irbis64);
                engine_SITE->moveToThread(thread_irbis64);
                connect(thread_irbis64, &QThread::started, engine_IRBIS64, &engine_irbis64::working);
                connect(engine_IRBIS64, &engine_irbis64::finished, thread_irbis64, &QThread::quit);
                connect(thread_site, &QThread::started, engine_SITE, &engine_site::flushToSite);
                connect(engine_SITE, &engine_site::finished, thread_site, &QThread::quit);
                if (rb->objectName() == "ozon") {
                    vector<pair<string, map<string, searchResult>>>::const_iterator it = bookVariants.begin();
                    while (it != bookVariants.end()) {
                        bool done = false;
                        if (it->first.find("ozon") != string::npos) {
                            map<string, searchResult>::const_iterator it2 = it->second.begin();
                            
                            while (it2 != it->second.end()) {
                                if (it2->first.find(page->windowTitle().toStdString()) != string::npos) {
                                    string qrcode = genPageName(1).front();
                                    engine_IRBIS64->setBookSearch(it2->second);
                                    engine_IRBIS64->setRancode(qrcode);
                                    engine_IRBIS64->setMAGIC_CODE(magiccode);
                                    engine_SITE->setSearchBook(it2->second);
                                    engine_SITE->setRancode(qrcode);
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
                    vector<pair<string, map<string, searchResult>>>::const_iterator it = bookVariants.begin();
                    while (it != bookVariants.end()) {
                        bool done = false;
                        if (it->first.find("livelib") != string::npos) {
                            map<string, searchResult>::const_iterator it2 = it->second.begin();
                            
                            while (it2 != it->second.end()) {
                                if (it2->first.find(page->windowTitle().toStdString()) != string::npos) {
                                    string qrcode = genPageName(1).front();
                                    engine_IRBIS64->setBookSearch(it2->second);
                                    engine_IRBIS64->setRancode(qrcode);
                                    engine_IRBIS64->setMAGIC_CODE(magiccode);
                                    engine_SITE->setSearchBook(it2->second);
                                    engine_SITE->setRancode(qrcode);
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
                    vector<pair<string, map<string, searchResult>>>::const_iterator it = bookVariants.begin();
                    while (it != bookVariants.end()) {
                        bool done = false;
                        if (it->first.find("litres") != string::npos) {
                            map<string, searchResult>::const_iterator it2 = it->second.begin();
                            
                            while (it2 != it->second.end()) {
                                if (it2->first.find(page->windowTitle().toStdString()) != string::npos) {
                                    string qrcode = genPageName(1).front();
                                    engine_IRBIS64->setBookSearch(it2->second);
                                    engine_IRBIS64->setRancode(qrcode);
                                    engine_IRBIS64->setMAGIC_CODE(magiccode);
                                    engine_SITE->setSearchBook(it2->second);
                                    engine_SITE->setRancode(qrcode);
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
                thread_irbis64->start();
//                thread_site->start();
            }
        }
        
                                    
         
    }*/
    
    
    
}

vector<string> mainwindow::genPageName(int count) 
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

string mainwindow::genMagicCode() {
    std::mt19937 gen(time(0));
    std::uniform_int_distribution<> uid(100000, 999999);
    std::uniform_int_distribution<> simb(48, 57);
    std::uniform_int_distribution<> pos(0, 5);
        
    return boost::lexical_cast<string>(uid(gen));
}


void mainwindow::slot_calculateBooksCount() {
    int count = 0;
    size_t pos = 0;
    if (frmMain.teISBN->toPlainText().length() > 0) {
        if (frmMain.teISBN->toPlainText().toStdString().find("\n") != string::npos) {
            do {
              ++count ;
              pos = frmMain.teISBN->toPlainText().toStdString().find("\n", pos+1);
            }while (pos != string::npos);
            
            frmMain.lblBooksRemain->setText(QString::number(count)) ;
        }
        else {
            frmMain.lblBooksRemain->setText("1");
        }
    }
    else {
        frmMain.lblBooksRemain->setText("0");
    }
}

void mainwindow::slot_bookStatusChanged(QString text) {
    frmMain.teStatus->insertPlainText(text + "\n");
}
