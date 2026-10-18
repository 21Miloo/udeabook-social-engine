#ifndef UDEABOOK_H
#define UDEABOOK_H

#include <string>
#include "DynamicList.h"
#include "adjacencymatrix.h"
#include "user.h"
#include "suggestion.h"
#include <random>

using namespace std;

class UdeaBook{

private:

    DynamicList<User*> users;     // todos los usuarios, ordenados por userName. UdeaBook es el dueño
    AdjacencyMatrix friendships;  // friendships[i][j] es true si los usuarios i y j son amigos
    User* currentUser;            // el usuario que inicio sesion, nullptr si nadie
    string dataPath;              // carpeta donde estan los .txt (ej: "data/")
    string basePath;              // ruta fija de las fotos (ej: "/users/udeabook")
    DynamicList<User*> showBestFriends; // los 3 amigos que se mostraran al iniciar sesion
    bool photosUpdated; // Valida si las fotos fueron cargadas o no
    mt19937 randomGenerator; // generador de numeros aleatorios de C++ (viene de <random>)




    // Carga de cada archivo
    bool loadUsers();
    bool loadFriendships();
    bool loadRequests();

    int findUserIndex(const string& userName) const; // busqueda binaria, -1 si no esta



    //Mejores amigos
    static int calculateWeight(int common);
    void selectWeighted(int f, const DynamicList<User*>& excluded, DynamicList<User*>& res);


    static void cleanLine(string& line); // quita el \r que dejan los archivos editados en Windows
    static int splitLine(const string& line, string* fields, int maxFields); // parte la linea por |

public:

    UdeaBook(const string& dataPath, const string& basePath); // Constructor
    ~UdeaBook(); // Destructor

    UdeaBook(const UdeaBook& other) = delete;            // no se permite copiar la red social
    UdeaBook& operator=(const UdeaBook& other) = delete; // (copiarla causaria double free de los usuarios)

    bool loadData(); // carga todos los archivos al iniciar

    // Sesion
    bool login(const string& userName, const string& password);
    void logout();
    User* getCurrentUser() const;

    // Busqueda de usuarios
    User* findUser(const string& userName) const;
    User* findUser(int index) const;
    int getUserCount() const;

    bool areFriends(User* a, User* b) const; // consulta la matriz en O(1)

    // Metodo que cuenta cuantos amigos tienen en comun user "A" y user "B"
    int countCommonFriends(User* a, User*b) const;

    // Mejores amigos
    void getBestFriends(DynamicList<User*>& res);
    bool updateBestFriends(DynamicList<User*>& res);

    // sugerencias de amistad
    void getSuggestions(int f, DynamicList<Suggestion>& res) const;

};

#endif // UDEABOOK_H
