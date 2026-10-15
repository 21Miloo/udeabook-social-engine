#ifndef DATE_H
#define DATE_H

class Date{

private:

    // Atributos
    int day;
    int month;
    int year;
    int hour;
    int minute;

public:

    // Constructores
    Date();
    Date(int d, int m, int y);
    Date(int d, int m, int y, int h, int min);
    Date(const Date& other); // Constructor de copia


    // Getters

    int getDay() const;

    int getMonth() const;

    int getYear() const;

    int getHour() const;

    int getMinute() const;



};





#endif // DATE_H
