#ifndef _DATE_H_
#define _DATE_H_

#include <string>
#include <cstring>
#include <iostream>
#include <stdlib.h>
//DD-MM-YYYY

class date {
private:
public:
    short unsigned int day;
    short unsigned int month;
    short unsigned int year;
    bool set; //if true, date exists, if not, no matter the int fields, this is a "-" date aka doesn't exist.

    date();
    date(std::string str);
    date(date &d);
    ~date();

    int get_day();
    int get_month();
    int get_year();

    void set_day(int d);
    void set_month(int m);
    void set_year(int y);

    std::string get_date_as_string();
    void print_date();
};
short int isLater(date d1, date d2); //0 => same date, 1 => d1 later d2, -1 => d2 later d1
bool isBetween(date d, date d1, date d2);
#endif