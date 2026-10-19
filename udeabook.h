#ifndef UDEABOOK_H
#define UDEABOOK_H

#include <string>
#include "DynamicList.h"
#include "adjacencymatrix.h"
#include "user.h"
#include "suggestion.h"
#include "message.h"
#include "privacy.h"
#include <random>

using namespace std;

// Resultado de enviar una solicitud de amistad, los vamos a restringir a un enum
enum RequestResult { REQUEST_SENT, REQUEST_TO_MYSELF, REQUEST_ALREADY_FRIENDS, REQUEST_ALREADY_SENT, REQUEST_PENDING_FROM_THEM };

// Resultado de enviar un mensaje
enum MessageResult { MESSAGE_SENT, MESSAGE_NOT_FRIENDS, MESSAGE_INVALID_TEXT };

class UdeaBook{

private:

    DynamicList<User*> users;     // todos los usuarios, ordenados por userName. UdeaBook es el dueño
    AdjacencyMatrix friendships;  // friendships[i][j] es true si los usuarios i y j son amigos
    DynamicList<Message*> messages; // todos los mensajes ordenados por fecha. UdeaBook es el dueño
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
    bool loadMessages();
    bool loadAlbums();
    bool loadPhotos();

    // Guardado de cada archivo
    bool saveFriendships() const;
    bool saveRequests() const;
    bool saveMessages() const;
    bool savePhotos() const; // los likes son lo unico de las fotos que cambia

    static bool textToPrivacy(const string& text, Privacy& privacy); // "PUBLIC" -> PUBLIC, false si no es valido
    static bool isNumber(const string& text); // true si el texto son solo digitos

    // marca la amistad de la matriz y en las 2 listas
    void makeFriends(User* a, User* b);

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
    bool saveData() const; // guarda los cambios al salir

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

    // Solicitudes de amistad
    RequestResult sendRequest(User* to);
    bool acceptRequest(User* from);
    bool rejectRequest(User* from);

    // Mensajes
    MessageResult sendMessage(User* to, const string& text);
    void getConversation(User* other, DynamicList<Message*>& res);
    int countUnreadFrom(User* from) const;
    int countUnreadMessages() const;
    void readNewMessages(DynamicList<Message*>& res);
    void searchMessagesByRange(const Date& start, const Date& end, DynamicList<Message*>& res) const;

    // sugerencias de amistad
    void getSuggestions(int f, DynamicList<Suggestion>& res) const;

    // Medicion de recursos: bytes de TODO lo que existe en la plataforma en este momento
    size_t calculateTotalMemory() const;

};

#endif // UDEABOOK_H
