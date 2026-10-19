#include "date.h"
#include <iomanip>
#include <chrono>

Date::Date(): day(1), month(1), year(1971), hour(0), minute(0){

}

Date::Date(int d, int m, int y) : Date(d, m, y, 0, 0){

}

Date::Date(int d, int m, int y, int h, int min): day(d), month(m), year(y), hour(h), minute(min){

}

Date::Date(const Date& other): day(other.day), month(other.month), year(other.year), hour(other.hour), minute(other.minute) {

}


// Getters
int Date::getDay() const {
    return day;
}

int Date::getMonth() const {
    return month;
}

int Date::getYear() const {
    return year;
}

int Date::getHour() const {
    return hour;
}

int Date::getMinute() const {
    return minute;
}

// Constructor que convierte un texto "aaaa-mm-dd hh:mm" en una fecha
Date::Date(const string& text) : Date() {

    if (text.length() < 10) return; // texto incompleto, se queda con la fecha por defecto

    year  = stoi(text.substr(0, 4)); // en el texto va primero el año
    month = stoi(text.substr(5, 2));
    day   = stoi(text.substr(8, 2));

    if (text.length() >= 16) { // si trae hora la leemos, si no queda en 00:00
        hour   = stoi(text.substr(11, 2));
        minute = stoi(text.substr(14, 2));
    }
}

// Operador = que copia los valores de otra fecha
Date& Date::operator=(const Date& other) {
    day = other.day;
    month = other.month;
    year = other.year;
    hour = other.hour;
    minute = other.minute;
    return *this;
}

// Metodo que convierte la fecha en un numero aaaammddhhmm
long long Date::toKey() const {
    return year * 100000000LL + month * 1000000LL + day * 10000LL + hour * 100LL + minute; // LL para que no se desborde el int
}

// Operador < que dice si esta fecha es anterior a otra
bool Date::operator<(const Date& other) const { return toKey() < other.toKey(); }

// Operador <= que dice si esta fecha es anterior o igual a otra
bool Date::operator<=(const Date& other) const { return toKey() <= other.toKey(); }

// Operador == que dice si dos fechas son exactamente iguales
bool Date::operator==(const Date& other) const { return toKey() == other.toKey(); }

// Operador << que muestra la fecha en formato aaaa-mm-dd hh:mm
ostream& operator<<(ostream& out, const Date& date) {
    out << setfill('0')
        << setw(4) << date.year << "-" << setw(2) << date.month << "-" << setw(2) << date.day << " "
        << setw(2) << date.hour << ":" << setw(2) << date.minute
        << setfill(' '); // devolvemos el relleno normal para no dañar lo que se imprima despues
    return out;
}

// Metodo que dice si un año es bisiesto
bool Date::isLeapYear(int y){
    return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0; // cada 4 años, menos los siglos que no son multiplos de 400
}

// Metodo que retorna la fecha y hora actual (hora de Colombia)
Date Date::now(){

    long long seconds = chrono::duration_cast<chrono::seconds>(
                            chrono::system_clock::now().time_since_epoch()).count(); // segundos desde el 1/1/1970 en hora UTC
    seconds -= 5 * 3600; // Colombia es UTC-5

    long long days = seconds / 86400;  // dias completos desde 1970
    int secondsToday = seconds % 86400; // segundos que van del dia de hoy

    int y = 1970;
    while (days >= (isLeapYear(y) ? 366 : 365)) { // quitamos años completos
        days -= isLeapYear(y) ? 366 : 365;
        y++;
    }

    int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(y)) monthDays[1] = 29;

    int m = 0;
    while (days >= monthDays[m]) { // quitamos meses completos
        days -= monthDays[m];
        m++;
    }

    return Date(days + 1, m + 1, y, secondsToday / 3600, (secondsToday % 3600) / 60); // dia y mes empiezan en 1, no en 0
}
