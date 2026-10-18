#include "user.h"

// Implementacion del Constructor por defecto
User::User() : index(-1){ // Un usuario por defecto no tiene fila en la matriz. si alguien usa por error significa que "No esta en ninguna posicion"

}

// Implementacion del Constructor con todos los datos
User::User(const string& userName, const string& password, const string& email, const string& profilePhotoPath, const string& country, const string& city, const Date& birthDate, const Date& registrationDate, int index): userName(userName), password(password), email(email),
    profilePhotoPath(profilePhotoPath), country(country), city(city),
    birthDate(birthDate), registrationDate(registrationDate), index(index)
{
}

// Getters
const string& User::getUserName() const { return userName; }
const string& User::getEmail() const { return email; }
const string& User::getProfilePhotoPath() const { return profilePhotoPath; }
const string& User::getCountry() const { return country; }
const string& User::getCity() const { return city; }
const Date& User::getBirthDate() const { return birthDate; }
const Date& User::getRegistrationDate() const { return registrationDate; }
int User::getIndex() const { return index; }

const DynamicList<User*>& User::getFriends() const { return friends; }
const DynamicList<User*>& User::getReceivedRequests() const { return receivedRequests; }

// Metodo que verifica si la clave ingresada es la correcta
bool User::verifyPassword(const string& password) const {
    return this->password == password; // this->password es el atributo, password es el parametro
}

// Metodo que agrega un amigo a la lista
void User::addFriend(User* u) {
    friends.add(u); // la matriz se marca en UdeaBook, aqui solo la lista
}

// Metodo que registra una solicitud recibida de u
void User::receiveRequest(User* u) {
    receivedRequests.add(u);
    u->sentRequests.add(this); // la misma solicitud queda registrada en el que la envio
}

// Metodo que dice si u me envio una solicitud
bool User::hasRequestFrom(User* u) const {
    return receivedRequests.contains(u);
}

// Metodo que dice si yo ya le envie una solicitud a u
bool User::hasSentRequestTo(User* u) const {
    return sentRequests.contains(u);
}

// Metodo que quita la solicitud de u hacia mi (al aceptarla)
void User::removeRequest(User* u) {
    int pos = receivedRequests.indexOf(u);
    if (pos != -1) receivedRequests.removeAt(pos);

    pos = u->sentRequests.indexOf(this);
    if (pos != -1) u->sentRequests.removeAt(pos); // tambien sale de las enviadas de u
}

// Operador == que compara dos usuarios por su nombre (es unico)
bool User::operator==(const User& other) const {
    return userName == other.userName;
}

// Operador << que muestra el usuario
ostream& operator<<(ostream& out, const User& user) {
    out << user.userName << " (" << user.city << ", " << user.country << ")";
    return out;
}