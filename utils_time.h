#ifndef _UTILS_TIME_H
#define _UTILS_TIME_H

#include <map>
#include <ctime>
#include <string>
#include <stdio.h>

using namespace std;

class CCalendar 
{
public:
    static string getFormatedTime(time_t t);
    static string getFormatedDate(time_t t);
    static string getCurFormatedTime();
    static string getCurFormatedDate();
    static time_t getTime(int daySpan,int hourSpan,int minSpan,int secSpan);
    static time_t getCurTime();
    static time_t getCurDateTime();
};

#endif 
