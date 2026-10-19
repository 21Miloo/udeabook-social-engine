#ifndef DATE_H
#define DATE_H

#include <string>
#include <iostream>

using namespace std;

class Date{

private:

    // Atributos
    int day;
    int month;
    int year;
    int hour;
    int minute;

    long long toKey() const; // convierte la fecha en un solo numero para poder comparar

public:

    // Constructores
    Date();
    Date(int d, int m, int y);
    Date(int d, int m, int y, int h, int min);
    Date(const string& text); // Constructor desde texto "aaaa-mm-dd hh:mm"
    Date(const Date& other); // Constructor de copia

    Date& operator=(const Date& other); // Asignacion


    // Getters

    int getDay() const;

    int getMonth() const;

    int getYear() const;

    int getHour() const;

    int getMinute() const;

    // Operadores de comparacion
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator==(const Date& other) const;

    static Date now();               // fecha y hora actual
    static bool isLeapYear(int y);   // true si el año es bisiesto

    friend ostream& operator<<(ostream& out, const Date& date); // Para imprimir y guardar en archivo



};





#endif // DATE_H
