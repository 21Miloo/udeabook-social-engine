#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#include "date.h"
#include "DynamicList.h"

using namespace std;

class Album;   // declaracion adelantada, solo guardamos punteros
class Message;


class User{


private:

    // Datos personales.
    string userName;
    string password;
    string email;
    string profilePhotoPath;
    string country;
    string city;
    Date birthDate;
    Date registrationDate;

    int index; // Fila donde se encuentra el usuario en la matriz de adyacencia y posicion en la lista de UdeaBook

    // Relaciones con los otros usuarios de la plataforma
    DynamicList<User*> friends;
    DynamicList<User*> receivedRequests; // Solicitudes pendientes que me enviaron
    DynamicList<User*> sentRequests; // solicitudes pendientes que yo envie

public:

    // Constructores
    User();

    User(const string& userName, const string& password, const string& email, const string& profilePhotoPath, const string& country, const string& city, const Date& birthDate, const Date& registrationDate, int index);

    //Getters
    const string& getUserName() const;

    const string& getEmail() const;

    const string& getProfilePhotoPath() const;

    const string& getCountry() const;

    const string& getCity() const;

    const Date& getBirthDate () const;

    const Date& getRegistrationDate() const;

    int getIndex() const;

    const DynamicList<User*>& getFriends() const;
    const DynamicList<User*>& getReceivedRequests() const;


    // importante no exponer la clave. funcion para verificar la pass
    bool verifyPassword(const string& password) const;

    // Amigos y solicitudes
    void addFriend(User* u);
    void receiveRequest(User* u); // El usuario U me mando una solicitud
    bool hasRequestFrom(User* u) const; // tengo un solicitud de:
    bool hasSentRequestTo(User* u) const; // Envie una solicitud A:
    void removeRequest(User* u);  // Eliminar la solicitud de U hacia a mi (esta es una operacion en los 2 sentidos)


    // Operadores

    bool operator==(const User& other) const;
    friend ostream& operator<<(ostream& out, const User& user);


};

#endif // USER_H
