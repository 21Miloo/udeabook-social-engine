#include "date.h"

Date::Date(): day(1), month(1), year(1971), hour(0), minute(0){

}

Date::Date(int d, int m, int y) : Date(d, m, y, 0, 0){

}

Date::Date(int d, int m, int y, int h, int min): day(d), month(m), year(y), hour(h), minute(min){

}

Date::Date(const Date& other): day(other.day), month(other.month), year(other.year), hour(other.hour), minute(other.minute) {

}

int Date::getDay() const { return day; }

int Date::getMonth() const { return month; }

int Date::getYear() const { return year; }

int Date::getHour() const { return hour; }

int Date::getMinute() const { return minute; }