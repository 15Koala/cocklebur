#include <string.h>
#include "utils_time.h"

string CCalendar::getFormatedTime(time_t t)
{
    struct tm *p;
    p = localtime(&t);
    char timestr[30];
    memset(timestr,0,30);
    sprintf(timestr,"%4d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

    string curTime=timestr;
    return curTime;
}
time_t CCalendar::getCurDateTime(){
    time_t now = CCalendar::getCurTime();
    time_t last_get_time = now - now % (3600 * 24) - 8*3600;
    return last_get_time;
}
string CCalendar::getFormatedDate(time_t t)
{
    struct tm *p;
    p = localtime(&t);
    char datestr[30];
    memset(datestr,0,30);
    sprintf(datestr,"%4d-%02d-%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);

    string curDate=datestr;
    return curDate;
}


string CCalendar::getCurFormatedTime()
{
    time_t t = time(NULL) ;

    struct tm *p;
    p = localtime(&t);
    char timestr[30];
    memset(timestr,0,30);
    sprintf(timestr,"%4d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

    string curTime=timestr;
    return curTime;
}

string CCalendar::getCurFormatedDate()
{
    time_t t = time(NULL) ;
    struct tm *p;
    p = localtime(&t);
    char datestr[30];
    memset(datestr,0,30);
    sprintf(datestr,"%4d-%02d-%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);

    string curDate=datestr;
    return curDate;
}
time_t CCalendar::getCurTime(){
    time_t t = time(NULL);
    return t;
}
time_t CCalendar::getTime(int daySpan,int hourSpan,int minSpan,int secSpan)
{
    time_t t = time(NULL) ;
    t-=(time_t) (daySpan*24*3600)+(hourSpan*3600)+minSpan*60+secSpan;
    return t;
}


