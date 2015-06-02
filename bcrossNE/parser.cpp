/* 
 * File:   parser.cpp
 * Author: demiin
 * 
 * Created on February 27, 2015, 1:05 PM
 */

#include <QtCore/qstring.h>

#include "parser.h"

parser::parser() {
}

parser::~parser() {
}

vector<string> parser::split(string lines, char simb) 
{
    vector <string> splited;
    size_t pos = 0;
    size_t new_pos = 0;
    while ((new_pos = lines.find(simb, pos)) != string::npos)
    {
        if (lines.substr(pos, new_pos-pos).length() > 0)
        {
            splited.push_back(lines.substr(pos, new_pos-pos).replace(lines.substr(pos, new_pos-pos).length()-1, 1,""));
        }
            pos = new_pos+1;
    }

    return splited;
}

vector<string> parser::split(string lines, string str) 
{
    vector <string> splited;
    size_t pos = 0;
    size_t new_pos = 0;
    while ((new_pos = lines.find(str, pos)) != string::npos)
    {
        if (lines.substr(pos, new_pos-pos).length() > 0)
        {
            splited.push_back(lines.substr(pos, new_pos-pos).replace(lines.substr(pos, new_pos-pos).length()-1, 1,""));
        }
            pos = new_pos+1;
    }

    return splited;
}

string parser::getAutor(string line) 
{
    boost::regex re(".*#([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\s\\.]*)%.*");
    boost::smatch result;
    if (boost::regex_match(line, result, re))
    {
        if (result[1] != " ")
            return result[1];
    }
    
    return "";
}

string parser::getBookName(string line) 
{
    boost::regex re("%([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\s\\.a-zA-Z\\-0-9;!?\\*\\&\\$'\":]*)%");
    boost::smatch result;
    string::const_iterator begin = line.begin();
    string::const_iterator end = line.end();
    
    if (boost::regex_search(begin, end, result, re))
    {
        if (result[1] != " ")
        {
            begin = result[1].second;
            return result[1];            
        }
    }
    
    return "";
}

string parser::getSeries(string line) 
{
    boost::regex re("%([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\s\\.a-zA-Z\\-0-9;!?\\*\\&\\$'\":]*)");
    boost::smatch result;
    string::const_iterator begin = line.begin();
    string::const_iterator end = line.end();
    size_t match = 0;

    while (boost::regex_search(begin, end, result, re))
    {
        if (result[1] != " ")
        {
            match++;
            begin = result[1].second;
            if (match == 2)
                return result[1];            
        }
    }
    
    return "";
}

vector<where> parser::getWhereIs(string line) 
{
    vector<where> whereis;
    where id;
    boost::regex re("([0-9])\\$([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\s\\.\\-0-9]*)\\$([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\s\\.\\-0-9]*)");
    boost::smatch result;
    string::const_iterator begin = line.begin();
    string::const_iterator end = line.end();

    while (boost::regex_search(begin, end, result, re))
    {
        if (result[1] != " ")
        {
//            cout << result[1] <<  "  ----  "<< result[2] <<  "   ----  " << result[3] << endl;
            begin = result[3].second;
            id.status = result[1];
            id.inv_numb = result[2];
            id.filial = result[3];
            whereis.push_back(id);
        }
    }    
    
    return whereis;
}

string parser::remove_all(string line, string to_remove) 
{
    size_t pos = 0;
    size_t new_pos = 0;
    while ((new_pos = line.find(to_remove, pos)) != string::npos)
    {
        line.replace(new_pos, 1, "");
        pos = new_pos+1;
    }
    return line;
}

string parser::removeAllAfter(string line, string simb) {
    if  (simb == "-") {
        if (line.find_first_of(simb) != string::npos) {
            line = line.replace(line.find_first_of(simb) - 1, line.length() - line.find_first_of(simb) + 1, "");
        }
    }
    else
    if (simb == " ") {
        if (line.find_first_of(simb) != string::npos) {
            line = line.replace(line.find_first_of(simb), line.length() - line.find_first_of(simb), "");
        }
    }
    return line;
}


string parser::getAuthID(string line) {
    boost::regex re("&&&&&(.*)$");
    boost::smatch result;
    if (boost::regex_search(line, result, re))
        return result[1];
    else
        return "";
}

vector<string> parser::getGetFIlialsReg(string line) {
    vector<string> regs;
    boost::regex re("#([0-9\\sа-яА-Я;]*)#");
    boost::smatch result;
    string::const_iterator begin = line.begin();    
    string::const_iterator end = line.end();    
    while (boost::regex_search(begin, end, result, re)) {
        regs.push_back(result[1]);
        begin = result[2].second;
    }
    return regs;
}

vector<string> parser::getBooksJournalsOwned(string line) {
    vector<string> regs;
    boost::regex re("([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ;\\.,\\-\\/a-zA-Z\\[\\]\":0-9\\s\\(\\)\\+\\!\\?]*)@");
    boost::smatch result;
    string::const_iterator begin = line.begin();    
    string::const_iterator end = line.end();    
    while (boost::regex_search(begin, end, result, re)) {
        regs.push_back(result[1]);
        begin = result[2].second;
    }
    return regs;
}

vector<string> parser::makeFields(string line) {
    vector<string> fields;
    size_t pos = 0;
    size_t begin = 0;
    while ((pos = line.find(';', pos+1)) != string::npos) {
    //  cout << line.substr(begin, pos - begin) << endl;
      fields.push_back(line.substr(begin, pos - begin));
      begin = pos+1;
    }
    //cout << line.substr(begin, pos - begin)   << endl;
    fields.push_back(line.substr(begin, pos - begin));
    
    return fields;
}

vector<string> parser::getBookJournalReturned(string line) 
{
    vector<string> returned;
    boost::regex re("([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ;\\.,\\-\\/a-zA-Z\\[\\]\":0-9\\s\\(\\)\\+\\!\\?]*)%");
    boost::smatch result;
    string::const_iterator begin = line.begin();    
    string::const_iterator end = line.end();    
    while (boost::regex_search(begin, end, result, re)) {
        returned.push_back(result[1]);
        begin = result[2].second;
    }
    return returned;
}

vector<string> parser::getLocate(string line) {
    vector<string> locate;
    line +="%";
    boost::regex re("([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ0-9\\s]*)([цЦгГбБоОдДиИфФсСлЛкКАаХх0-9\\s]*)(\\,|%)");
    boost::smatch result;
    string::const_iterator begin = line.begin();    
    string::const_iterator end = line.end();    
    while (boost::regex_search(begin, end, result, re)) {
        locate.push_back(result[1]);
        begin = result[3].second;
    }
    
    return locate;
}

string parser::getMFN(string line) {
    boost::regex re("^(\\d{1,})#");
    boost::smatch result;
    if (boost::regex_search(line, result, re)) {
        return result[1];
    }
    
    return "";
}

string parser::getCodeInBase(string line, string invnumb) {
    boost::regex re_mass(".*\\^[Aa]([0-9\\(\\)\\.йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\-\\=\\/]*).*\\^[Ff]([\\*]*).*\\^[Hh](\\d{8}).*");
    boost::regex re_a(".*\\^[Aa]([0-9\\(\\)\\.йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\-\\=\\/]*).*") ;
    boost::regex re_h(".*\\^[Hh](\\d{8}).*");
    boost::regex re_f(".*\\^[Ff]([\\*]*).*");
    
    boost::smatch result;

    if (boost::regex_search(line, result, re_f)) {
        if (boost::regex_search(line, result, re_h)) {
            if (result[1] == invnumb) {
                if (boost::regex_search(line, result, re_a)) {
                    return result[1];
                }
            }
        }
    }
    return "";
}

string parser::getBase(string line, string invnumb) {
    boost::regex re_g(".*\\^[Gg]([a-zA-Z]*).*") ;
    boost::regex re_h(".*\\^[Hh](\\d{8}).*");
    boost::regex re_f(".*\\^[Ff]([\\*]*).*");
    
    boost::smatch result;

    if (boost::regex_search(line, result, re_f)) {
        if (boost::regex_search(line, result, re_h)) {
            if (result[1] == invnumb) {
                if (boost::regex_search(line, result, re_g)) {
                    return result[1];
                }
            }
        }
    }
    return "";
}


string parser::makeReturn_book(vector<string> book, string invnumb) {
    vector<string> newBook;
    string new_record;
    boost::regex re_a(".*\\^[Aa](\\d).*") ;
    boost::regex re_h(".*\\^[Hh](\\d{8}).*");
    boost::smatch result;

    for (size_t i = 11; i < book.size(); i++) {
        newBook.push_back(book.at(i));
    }
    
    for(string &line: newBook) {
        if (boost::regex_search(line, result, re_h)) {
            if (result[1] == invnumb) {
                size_t pos;
                if ((pos = line.find("^A")) != string::npos) {
                    line = line.replace(pos, 3, "^A0");
                }
                else if ((pos = line.find("^a")) != string::npos) {
                    line = line.replace(pos, 3, "^A0");
                }
            }
        }
        new_record += line + "\x1F\x1E";        
    }
    
    return new_record;
}

string parser::makeReturn_reader(vector<string> reader, string invnumb) {
    vector<string> newReader;
    string new_record;
    time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm utc_tm = *gmtime(&tt);
    string year = boost::lexical_cast<string>(utc_tm.tm_year + 1900);
    string month = (utc_tm.tm_mon + 1 < 10 ? "0" + boost::lexical_cast<string>(utc_tm.tm_mon + 1) : boost::lexical_cast<string>(utc_tm.tm_mon + 1));
    string day = (utc_tm.tm_mday < 10 ? "0" + boost::lexical_cast<string>(utc_tm.tm_mday) : boost::lexical_cast<string>(utc_tm.tm_mday));
    string curDate = year + month + day;
    
    boost::regex re_h(".*\\^[Hh](\\d{8}).*");
    boost::regex re_f(".*\\^[Ff]([\\*]*).*");
    boost::smatch result;

    for (size_t i = 11; i < reader.size(); i++) {
        newReader.push_back(reader.at(i));
    }
    
    
    for (string &line: newReader) {
        if (boost::regex_search(line, result, re_h)) {
            if (result[1] == invnumb) { // найден инвентарь
                size_t pos;
                if (boost::regex_search(line, result, re_f)) { // книга еще не возвращена
                    if ((pos = line.find("^F")) != string::npos) {
                        line = line.replace(pos, 8, "^F" + curDate);
                    }
                    else if ((pos = line.find("^f")) != string::npos) {
                        line = line.replace(pos, 8, "^F" + curDate);                        
                    }
                }
            }
        }
        new_record += line + "\x1F\x1E";
    }
    return new_record;
}

string parser::getCurYear() {
    time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm utc_tm = *gmtime(&tt);
    return boost::lexical_cast<string>(utc_tm.tm_year + 1900);
}

string parser::getCurMonth() {
    time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm utc_tm = *gmtime(&tt);
    return (utc_tm.tm_mon + 1 < 10 ? "0" + boost::lexical_cast<string>(utc_tm.tm_mon + 1) : boost::lexical_cast<string>(utc_tm.tm_mon + 1));
}

string parser::getCurDay() {
    time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm utc_tm = *gmtime(&tt);
    return (utc_tm.tm_mday < 10 ? "0" + boost::lexical_cast<string>(utc_tm.tm_mday) : boost::lexical_cast<string>(utc_tm.tm_mday));
}

string parser::getCurTime(string delimiter) {
    time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm utc_tm = *gmtime(&tt);
    return (utc_tm.tm_hour + 4 < 10 ? "0" + boost::lexical_cast<string>(utc_tm.tm_hour + 4) : boost::lexical_cast<string>(utc_tm.tm_hour + 4))+ delimiter + 
               (utc_tm.tm_min < 10 ? "0" + boost::lexical_cast<string>(utc_tm.tm_min) : boost::lexical_cast<string>(utc_tm.tm_min))+ delimiter + 
               (utc_tm.tm_sec < 10 ? "0" + boost::lexical_cast<string>(utc_tm.tm_sec) : boost::lexical_cast<string>(utc_tm.tm_sec));
}

string parser::getCurDayWeek() {
    time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm utc_tm = *gmtime(&tt);
    switch (utc_tm.tm_wday) {
        case 1: {
            return "Mon";
        }
        case 2: {
            return "Tue";
        }
        case 3: {
            return "Wed";
        }
        case 4: {
            return "Thu";
        }
        case 5: {
            return "Fri";
        }
        case 6: {
            return "Sat";
        }
        case 0: {
            return "Sun";
        }
    }
    return "";
}

string parser::getCurMonYear() {
    time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm utc_tm = *gmtime(&tt);
    switch (utc_tm.tm_mon) {
        case 1: {
            return "Feb";
        }
        case 2: {
            return "Mar";
        }
        case 3: {
            return "Apr";
        }
        case 4: {
            return "May";
        }
        case 5: {
            return "Jun";
        }
        case 6: {
            return "Jul";
        }
        case 7: {
            return "Aug";
        }
        case 8: {
            return "Sep";
        }
        case 9: {
            return "Oct";
        }
        case 10: {
            return "Nov";
        }
        case 11: {
            return "Dec";
        }
        case 0: {
            return "Jan";
        }
    }
    return "";

}

string parser::getFullPath(string line) {
    //boost::regex re(".*href=\"\\/([\\w\\/]*)\".*");
    boost::regex re(".*href=\"\\/(context\\/detail\\/[\\w\\/]*)\".*");
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "") {
            return result[1];
        }
    }
    
    re = ".*Location: http:\\/\\/www.litres.ru\\/([\\w\\/\\-]*).*";
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "") {
            return result[1];
        }
    }

    re = ".*ru\\/([\\w\\/]*).*";
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "") {
            return result[1];
        }
    }
        
    return "";
}

string parser::getImg(string line) {
    boost::regex re(".*\" src=\"([\\w\\.\\/\\_\\:]*)\".*");
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "")  {
            return string(result[1]).replace(string(result[1]).find_last_of("_"), 4, "").replace(0, 7, "");
        }
    }
    
    re = ".*PreviewBig\":\"\\/\\/([\\w\\.\\/]*)\".*";
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "") {
            return result[1];
        }
    }
    
    
    return "";
}

string parser::getBook_autor(string line) {
    boost::regex re(".*title='([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\.\\/\\_\\:\\s\\,\\-\\!\\?\\;a-zA-Z]*)'.*");
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "") {
            return result[1];
        }
    }
    
    re = ".*title=\"([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\.\\/\\_\\:\\s\\,\\-\\!\\?\\;a-zA-Z]*)\".*";
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "") {
            return result[1];
        }
    }

    re = ".*itemprop=\"author\">([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\.\\/\\_\\:\\s\\,\\-\\!\\?\\;a-zA-Z]*).*";
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "") {
            if (string(result[1])[0] == ' ') {
                string tmp =  string(result[1]).substr(1, string(result[1]).length() - 1);
                if (tmp[tmp.length() - 1] == '\x0D') 
                    tmp = tmp.substr(0, tmp.length() - 1);
                return tmp;
            }
            else
                return result[1];
        }
    }
    
    return "";
}

string parser::getBook_name(string line) {
    boost::regex re(".*itemprop=\"name\">([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ0-9\\.\\/\\_\\:\\s]*).*");
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "") {
            return result[1];
        }
    }
    
    re = ".*prodName\":\"([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ0-9\\s\\.\\/\\_\\:]*).*";
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "") {
            return result[1];
        }
    }
    
    return "";
}

string parser::getBook_descr(string line) {
    boost::regex re(".*og:description\" content=\"([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\.\\/\\_\\:\\s\\,\\!\\?\\;a-zA-Z\\–\\-0-9\"\\(\\)«»]*).*");
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "") {
            return result[1];
        }
    }
    
    re = ".*\"about\">([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\.\\/\\_\\:\\s\\,\\-\\!\\?\\;a-zA-Z\\&0-9\"\\(\\)«»]*).*";
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "") {
            return result[1];
        }
    }
    
    re = ".*<\\!\\-\\- Data\\[ANNOTATION\\] \\-\\-> <p>([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\s\\?\\!\\,\\-\\.\\:\\;\\'0-9\"\\(\\)«»\\–]*).*";
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "") {
            return result[1];
        }
    }

    re = ".*<\\!\\-\\- Data\\[ANNOTATION\\] \\-\\-> ([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\s\\?\\!\\,\\-\\.\\:\\;\\'0-9\"\\(\\)«»\\–]*).*";
    if (boost::regex_match(line, result, re)) {
        if (result[1] != "") {
            return result[1];
        }
    }
    
    
    return "";
}

#ifndef _WIN32
string parser::iconv_recode(string from, string to, string text)
{
    iconv_t cnv = iconv_open(to.c_str(), from.c_str());
    if (cnv == (iconv_t) - 1)
    {
        iconv_close(cnv);
        return "";
    }
    char *outbuf;
    if ((outbuf = (char *) malloc(text.length()*2 + 1)) == NULL)
    {
        iconv_close(cnv);
        return "";
    }
    char *ip = (char *) text.c_str(), *op = outbuf;
    size_t icount = text.length(), ocount = text.length()*2;

    if (iconv(cnv, &ip, &icount, &op, &ocount) != (size_t) - 1)
    {
        outbuf[text.length()*2 - ocount] = '\0';
        text = outbuf;
    }
    else
    {
        text = "";
    }

    free(outbuf);
    iconv_close(cnv);
    return text;
}
#endif

string parser::maxMFN(string line) 
{
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    string lastResult;
//    cout << line << endl;
    boost::regex re("(\\d{3,})");
    while (boost::regex_search(begin, end,  result, re))
    {
        begin = result[1].second;
        if (boost::lexical_cast<string>(result[1]).length() > 0)
            lastResult = boost::lexical_cast<string>(result[1]);
    }    
    return lastResult;
}

string parser::replaceAllSpace(string line, string forwhat) {
    size_t pos = 0;
    size_t new_pos = 0;
    while ((new_pos = line.find(" ", pos)) != string::npos)
    {
        line.replace(new_pos, 1, forwhat);
        pos = new_pos+1;
    }
    return line;
}

string parser::getFormID(string line) {
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    string id;

    boost::regex re("name=\"form_build_id\"\\svalue=\"([a-zA-Z0-9\\-_]*)\"");
    while (boost::regex_search(begin, end,  result, re))
    {
        begin = result[2].second;
                
        //if (boost::lexical_cast<string>(result[1]) == "form_build_id")
        id = boost::lexical_cast<string>(result[1]);
    }    
    
    return id;    
}

string parser::getFormToken(string line) {
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    string token;
    
    boost::regex re("name=\"form_token\"\\svalue=\"([a-zA-Z0-9\\-_]*)\"");
    while (boost::regex_search(begin, end,  result, re))
    {
        begin = result[2].second;
        if (result[1] != "")
        {
            token = boost::lexical_cast<string>(result[1]);
            break;
        }  
    }    
    
    return token;    
}

string parser::getCookie(string line) {
    return line.substr(12, 80);
}
