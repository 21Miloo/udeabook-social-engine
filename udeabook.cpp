#include "udeabook.h"
#include <fstream>
#include <iostream>

// Implementacion del Constructor
UdeaBook::UdeaBook(const string& dataPath, const string& basePath)
    : currentUser(nullptr), dataPath(dataPath), basePath(basePath)
{
}

// Implementacion del Destructor
UdeaBook::~UdeaBook(){

    for (int i = 0; i < users.getSize(); i++) {
        delete users[i]; // UdeaBook creo cada usuario con new, entonces UdeaBook los borra
    }
    // la lista y la matriz se liberan solas con sus propios destructores
}

// Metodo que quita el \r del final de una linea
void UdeaBook::cleanLine(string& line){

    if (!line.empty() && line[line.length() - 1] == '\r') {
        line.erase(line.length() - 1); // si el archivo se guardo con saltos de Windows, el \r quedaria pegado al ultimo campo
    }
}

// Metodo que parte una linea por | y guarda cada campo en fields. Retorna cuantos campos encontro
int UdeaBook::splitLine(const string& line, string* fields, int maxFields){

    int count = 0;
    size_t start = 0;

    while (count < maxFields - 1) {
        size_t pos = line.find('|', start);
        if (pos == string::npos) break; // ya no hay mas separadores

        fields[count] = line.substr(start, pos - start);
        count++;
        start = pos + 1; // seguimos despues del |
    }

    fields[count] = line.substr(start); // el ultimo campo es todo lo que queda (en mensajes el texto puede tener |)
    count++;
    return count;
}

// Metodo que carga users.txt
bool UdeaBook::loadUsers(){

    ifstream file(dataPath + "users.txt");
    if (!file.is_open()) {
        cout << "No se pudo abrir " << dataPath << "users.txt" << endl;
        return false;
    }

    string line;
    if (!getline(file, line)) return false; // archivo vacio
    cleanLine(line);
    int total = stoi(line); // la primera linea dice cuantos usuarios hay

    users = DynamicList<User*>(total);       // creamos la lista y la matriz del tamano exacto
    friendships = AdjacencyMatrix(total);    // asi nunca tienen que hacer resize durante la carga

    string fields[7];
    while (getline(file, line)) {
        cleanLine(line);
        if (line.empty()) continue;
        if (splitLine(line, fields, 7) != 7) continue; // linea mal formada, la saltamos

        // la busqueda binaria solo funciona si el archivo viene ordenado, lo verificamos al cargar
        if (users.getSize() > 0 && !(users[users.getSize() - 1]->getUserName() < fields[0])) {
            cout << "users.txt no esta ordenado por nombre de usuario (" << fields[0] << ")" << endl;
            return false;
        }

        int index = friendships.addNode(); // la fila de la matriz coincide con la posicion en la lista
        string profilePhoto = basePath + "/" + fields[0] + "/albums/profile.png"; // la ruta se deduce, no se guarda

        // archivo: userName|password|email|birthDate|city|country|registrationDate
        users.add(new User(fields[0], fields[1], fields[2], profilePhoto, fields[5], fields[4],
                           Date(fields[3]), Date(fields[6]), index));
    }
    return true;
}

// Metodo que carga friendships.txt
bool UdeaBook::loadFriendships(){

    ifstream file(dataPath + "friendships.txt");
    if (!file.is_open()) return true; // sin archivo simplemente no hay amistades

    string line;
    string fields[2];
    while (getline(file, line)) {
        cleanLine(line);
        if (line.empty() || splitLine(line, fields, 2) != 2) continue;

        User* a = findUser(fields[0]);
        User* b = findUser(fields[1]);
        if (a == nullptr || b == nullptr || a == b) continue; // usuario que no existe
        if (areFriends(a, b)) continue; // linea repetida, no la agregamos dos veces a las listas

        friendships.addFriendship(a->getIndex(), b->getIndex()); // la matriz para consultar en O(1)
        a->addFriend(b); // las listas para recorrer solo los amigos reales
        b->addFriend(a);
    }
    return true;
}

// Metodo que carga requests.txt
bool UdeaBook::loadRequests(){

    ifstream file(dataPath + "requests.txt");
    if (!file.is_open()) return true; // sin archivo no hay solicitudes pendientes

    string line;
    string fields[2];
    while (getline(file, line)) {
        cleanLine(line);
        if (line.empty() || splitLine(line, fields, 2) != 2) continue;

        User* sender = findUser(fields[0]);
        User* receiver = findUser(fields[1]);
        if (sender == nullptr || receiver == nullptr || sender == receiver) continue;
        if (areFriends(sender, receiver) || receiver->hasRequestFrom(sender)) continue; // ya son amigos o esta repetida

        receiver->receiveRequest(sender); // queda registrada en los dos usuarios
    }
    return true;
}

// Metodo que carga todos los datos al iniciar el programa
bool UdeaBook::loadData(){

    if (!loadUsers()) return false; // sin usuarios no se puede cargar nada mas
    if (!loadFriendships()) return false;
    if (!loadRequests()) return false;
    return true;
}

// Metodo que busca la posicion de un usuario por su nombre usando busqueda binaria
int UdeaBook::findUserIndex(const string& userName) const {

    int low = 0;
    int high = users.getSize() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2; // asi no se desborda si low + high fuera muy grande
        const string& name = users[mid]->getUserName();

        if (name == userName) return mid;
        if (name < userName) low = mid + 1; // el buscado esta en la mitad derecha
        else high = mid - 1;                // el buscado esta en la mitad izquierda
    }
    return -1; // no existe
}

// Metodo que busca un usuario por su nombre
User* UdeaBook::findUser(const string& userName) const {

    int pos = findUserIndex(userName);
    if (pos == -1) return nullptr;
    return users[pos];
}

// Metodo que busca un usuario por su indice
User* UdeaBook::findUser(int index) const {

    if (index < 0 || index >= users.getSize()) return nullptr;
    return users[index];
}

// Metodo que retorna cuantos usuarios hay
int UdeaBook::getUserCount() const {
    return users.getSize();
}

// Metodo que dice si dos usuarios son amigos
bool UdeaBook::areFriends(User* a, User* b) const {
    return friendships.areFriends(a->getIndex(), b->getIndex());
}

// Metodo que inicia sesion si el usuario existe y la clave es correcta
bool UdeaBook::login(const string& userName, const string& password){

    User* u = findUser(userName);
    if (u == nullptr || !u->verifyPassword(password)) {
        return false; // no decimos cual de los dos fallo, por seguridad
    }
    currentUser = u;
    return true;
}

// Metodo que cierra la sesion
void UdeaBook::logout(){
    currentUser = nullptr;
}

// Metodo que retorna el usuario que inicio sesion
User* UdeaBook::getCurrentUser() const {
    return currentUser;
}
