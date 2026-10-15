#include "date.h"
#include <iomanip>

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
