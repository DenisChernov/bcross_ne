/* 
 * File:   parser.h
 * Author: demiin
 *
 * Created on February 27, 2015, 1:05 PM
 */

#ifndef PARSER_H
#define	PARSER_H

#include <iostream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#ifndef _WIN32
    #include <iconv.h>
#endif
using namespace std;

struct where
{
    string status;
    string inv_numb;
    string filial;
};

struct book
{
    vector<string> fulldata;
    string autor;
    string bookname;
    vector<where> whereis;
    string series;
};

struct searchResult {
    string ISBN_Autor;
    string picAddr;
    string autor;
    string bookname;
    string descr;
};

class parser {
public:
    parser();
    virtual ~parser();
    vector<string> split(string lines, char simb);
    vector<string> split(string lines, string str);
    string getAutor(string line);
    string getBookName(string line);
    string getSeries(string line);
    vector<where> getWhereIs(string line);
    string remove_all(string line, string to_remove);
    string getAuthID(string line);
    vector<string> getGetFIlialsReg(string line);
    vector<string> getBooksJournalsOwned(string line);
    vector<string> getBookJournalReturned(string line);
    vector<string> makeFields(string line);
    vector<string> getLocate(string line);
    string makeReturn_book(vector<string> book, string invnumb);
    string makeReturn_reader(vector<string> reader, string invnumber);
    book parsbooks;
    string getMFN(string line);
    string getCodeInBase(string line, string invnumb);
    string getBase(string line, string invnumb);
    string removeAllAfter(string line, string simb);
    string getCurYear();
    string getCurMonth();
    string getCurDay();
    string getCurTime(string delimiter);
    string getCurDayWeek();
    string getCurMonYear();
    string getFullPath(string line);
    string getImg(string line);
    string getBook_name(string line);
    string getBook_autor(string line);
    string getBook_descr(string line);
#ifndef _WIN32    
    string iconv_recode(string from, string to, string text);
#endif
    string maxMFN(string line);
    string replaceAllSpace(string line, string forwhat);
    
    string getFormID(string line);
    string getFormToken(string line);
    string getCookie(string line);
    
private:
    boost::smatch result;
};

#endif	/* PARSER_H */

