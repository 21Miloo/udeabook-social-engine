#include "udeabook.h"
#include <fstream>
#include <iostream>

// Implementacion del Constructor
UdeaBook::UdeaBook(const string& dataPath, const string& basePath): currentUser(nullptr), dataPath(dataPath), basePath(basePath), photosUpdated(false), randomGenerator(random_device{}())
{
}

// Implementacion del Destructor
UdeaBook::~UdeaBook(){

    for (int i = 0; i < users.getSize(); i++) {
        delete users[i]; // UdeaBook creo cada usuario con new, entonces UdeaBook los borra
    }
    for (int i = 0; i < messages.getSize(); i++) {
        delete messages[i]; // los mensajes tambien los crea UdeaBook
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

        makeFriends(a, b);
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
    if (!loadMessages()) return false;
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

    //Usamos estos metodos, porque si alguien cierra sesion y entra otro usuario, el nuevo debe tener su propia actualizacion dispo y no
    // heredar los amigos mostrados de la anterior sesion
    showBestFriends.clear();
    photosUpdated = false;

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

// Metodo que cuenta cuantos amigos tienen en comun user "A" y user "B"
int UdeaBook::countCommonFriends(User* a, User* b) const {

    int count = 0;
    const DynamicList<User*>& friendsA = a ->getFriends(); // referencia, no copiamos la lista (Esta operacion nos ahorra memoria)

    for (int i = 0; i < friendsA.getSize(); ++i) {

        if (areFriends(friendsA[i], b)) count ++; // El amigo de A tambien es amigo de B

    }

    return count;

}


// Metodo que retorna el peso de un amigo segun los amigos en comun (multiplicamos por 2 para no usar decimales)
int UdeaBook:: calculateWeight(int common){


    if (common > 20) return 7; // 3.5 x 2
    if (common > 10) return 5; // 2.5 x 2
    return 2;   // si common es 1, entonces 1x2
}



// Metodo que escoge n amigos al azar segun su peso, sin repetir y sin tomar los de excluded
void UdeaBook:: selectWeighted(int f, const DynamicList<User*>& excluded, DynamicList<User*>& res){

    const DynamicList<User*>& friends = currentUser->getFriends(); // tomamos la lista de amigos
    int total = friends.getSize(); // extraemos la cantidad numerica de la lista de amigos

    int* weights = new int[total]; // Le damos un peso por cada amigo, en la misma posicion
    int totalWeight = 0;

    for(int i = 0; i <total; i++){

        if (excluded.contains(friends[i])) {
            weights[i] = 0; // le damos peso 0 por lo tanto nunca saldria peso // 0 = nunca sale
        } else {
            weights[i] = calculateWeight(countCommonFriends(currentUser, friends[i]));
        }
        totalWeight += weights[i];

    }

    for (int chosen = 0; chosen < f && totalWeight > 0; ++chosen) {

        uniform_int_distribution<int> range(0, totalWeight - 1);
        int r = range(randomGenerator); // numero al azar entre 0 y totalWeight - 1

        int accumulated = 0;
        for (int i = 0; i < total; i++) {
            accumulated += weights[i];
            if (r < accumulated) {         // r cayo en el "pedazo" de este amigo
                res.add(friends[i]);
                totalWeight -= weights[i];
                weights[i] = 0;            // lo sacamos para que no se repita en el mismo set
                break;
            }
        }
    }

    delete[] weights; // si no, cada llamada deja el arreglo perdido en memoria
}

// Metodo que escoge los 3 mejores amigos que se muestran al iniciar sesion
void UdeaBook:: getBestFriends(DynamicList<User*>& res){


    DynamicList<User*> nobody; // la primera vez no excluimos a nadie
    selectWeighted(3, nobody, res);
    showBestFriends = res; // los guardamos para que la actualizacion muestre otros

}


// Metodo que cambia los 3 mejores amigos por otros, solo se permite una vez
bool UdeaBook:: updateBestFriends(DynamicList<User*>& res){

    if(photosUpdated) return false; // ya se uso la unica actualizacion
    photosUpdated = true;

    selectWeighted(3, showBestFriends, res); // primero intentamos con amigos que no se hayan mostrado

    if(res.getSize() < 3) {

        DynamicList<User*> alreadyChosen = res;
        selectWeighted(3 - res.getSize(), alreadyChosen, res);

    }

    showBestFriends = res;

    return true;

}

// Metodo que calcula f sugerencias de amistad: amigos de mis amigos ordenados por amigos en comun
void UdeaBook::getSuggestions(int k, DynamicList<Suggestion>& res) const {

    User* me = currentUser;
    int* common = new int[users.getSize()](); // un contador por usuario, todos en 0
    DynamicList<User*> candidates;              // cada candidato una sola vez

    const DynamicList<User*>& myFriends = me->getFriends();
    for (int i = 0; i < myFriends.getSize(); i++) {

        const DynamicList<User*>& friendsOfFriend = myFriends[i]->getFriends();
        for (int j = 0; j < friendsOfFriend.getSize(); j++) {

            User* candidate = friendsOfFriend[j];
            if (candidate == me || areFriends(me, candidate)) continue; // yo o alguien que ya es mi amigo

            if (common[candidate->getIndex()] == 0) candidates.add(candidate); // primera vez que lo vemos
            common[candidate->getIndex()]++; // myFriends[i] es un amigo en comun entre candidate y yo
        }
    }

    DynamicList<Suggestion> all(candidates.getSize());
    for (int i = 0; i < candidates.getSize(); i++) {
        all.add(Suggestion(candidates[i], common[candidates[i]->getIndex()]));
    }
    delete[] common;

    // seleccion parcial: solo ordenamos los k primeros, no toda la lista
    for (int i = 0; i < k && i < all.getSize(); i++) {
        int best = i;
        for (int j = i + 1; j < all.getSize(); j++) {
            if (all[j] > all[best]) best = j; // operator> de Suggestion compara los amigos en comun
        }
        Suggestion temp = all[i];
        all[i] = all[best];
        all[best] = temp;
        res.add(all[i]);
    }
}

// Metodo que deja a a y b como amigos en la matriz y en las listas de los dos
void UdeaBook::makeFriends(User* a, User* b){

    friendships.addFriendship(a->getIndex(), b->getIndex()); // la matriz para consultar en O(1)
    a->addFriend(b); // las listas para recorrer solo los amigos reales
    b->addFriend(a);
}


// Metodo que envia una solicitud de amistad del usuario actual a "to"
RequestResult UdeaBook::sendRequest(User* to){

    User* me = currentUser;

    if (to == me) return REQUEST_TO_MYSELF;                    // nadie se puede enviar solicitud a si mismo
    if (areFriends(me, to)) return REQUEST_ALREADY_FRIENDS;    // ya son amigos
    if (me->hasSentRequestTo(to)) return REQUEST_ALREADY_SENT; // no repetimos la misma solicitud
    if (me->hasRequestFrom(to)) return REQUEST_PENDING_FROM_THEM; // el otro ya me la envio, toca aceptarla

    to->receiveRequest(me); // queda en las recibidas de "to" y en las enviadas mias
    return REQUEST_SENT;
}

// Metodo que acepta la solicitud que "from" le envio al usuario actual
bool UdeaBook::acceptRequest(User* from){

    if (!currentUser->hasRequestFrom(from)) return false; // no hay solicitud de ese usuario

    currentUser->removeRequest(from); // sale de mis recibidas y de las enviadas de from
    makeFriends(currentUser, from);
    return true;
}

// Metodo que rechaza la solicitud que "from" le envio al usuario actual
bool UdeaBook::rejectRequest(User* from){

    if (!currentUser->hasRequestFrom(from)) return false;

    currentUser->removeRequest(from); // solo se borra la solicitud, no se crea la amist
    return true;
}

// Metodo que guarda friendships.txt
bool UdeaBook::saveFriendships() const {

    ofstream file(dataPath + "friendships.txt"); // ofstream borra el archivo y lo escribe desde cero
    if (!file.is_open()) return false;

    for (int i = 0; i < users.getSize(); i++) {
        const DynamicList<User*>& friends = users[i]->getFriends();

        for (int j = 0; j < friends.getSize(); j++) {
            if (friends[j]->getIndex() > i) { // cada pareja una sola vez: solo cuando el amigo va despues de mi
                file << users[i]->getUserName() << "|" << friends[j]->getUserName() << '\n';
            }
        }
    }
    return true;
}

// Metodo que guarda requests.txt
bool UdeaBook::saveRequests() const {

    ofstream file(dataPath + "requests.txt");
    if (!file.is_open()) return false;

    for (int i = 0; i < users.getSize(); i++) {
        const DynamicList<User*>& received = users[i]->getReceivedRequests();

        for (int j = 0; j < received.getSize(); j++) {
            file << received[j]->getUserName() << "|" << users[i]->getUserName() << '\n'; // formato: sender|receiver
        }
    }
    return true;
}

// Metodo que guarda todos los datos que pueden cambiar durante la ejecucion
bool UdeaBook::saveData() const {

    bool ok = true;

    if (!saveFriendships()) {
        cout << "No se pudo guardar " << dataPath << "friendships.txt" << endl;
        ok = false;
    }
    if (!saveRequests()) {
        cout << "No se pudo guardar " << dataPath << "requests.txt" << endl;
        ok = false; // seguimos intentando con los demas aunque uno falle
    }
    if (!saveMessages()) {
        cout << "No se pudo guardar " << dataPath << "messages.txt" << endl;
        ok = false;
    }
    return ok;
}

// Metodo que carga messages.txt
bool UdeaBook::loadMessages(){

    ifstream file(dataPath + "messages.txt");
    if (!file.is_open()) return true; // sin archivo no hay mensajes

    string line;
    string fields[5];
    while (getline(file, line)) {
        cleanLine(line);
        if (line.empty() || splitLine(line, fields, 5) != 5) continue; // el texto es el ultimo campo, puede tener |

        User* sender = findUser(fields[0]);
        User* receiver = findUser(fields[1]);
        if (sender == nullptr || receiver == nullptr || sender == receiver) continue;
        if (!Message::isValidText(fields[4])) continue; // vacio o mas de 512 caracteres

        Message* m = new Message(sender, receiver, fields[4], Date(fields[2]));
        if (fields[3] == "1") m->markAsRead(); // el constructor lo crea como no leido
        messages.add(m); // el archivo viene ordenado por fecha, la lista queda ordenada igual
    }
    return true;
}

// Metodo que guarda messages.txt
bool UdeaBook::saveMessages() const {

    ofstream file(dataPath + "messages.txt");
    if (!file.is_open()) return false;

    for (int i = 0; i < messages.getSize(); i++) {
        const Message* m = messages[i];
        file << m->getSender()->getUserName() << "|" << m->getReceiver()->getUserName() << "|"
             << m->getSentDate() << "|" << (m->isRead() ? 1 : 0) << "|" << m->getText() << '\n'; // el texto de ultimo
    }
    return true;
}

// Metodo que envia un mensaje del usuario actual a "to"
MessageResult UdeaBook::sendMessage(User* to, const string& text){

    if (!areFriends(currentUser, to)) return MESSAGE_NOT_FRIENDS; // solo se puede escribir a los amigos
    if (!Message::isValidText(text)) return MESSAGE_INVALID_TEXT;  // vacio o mas de 512 caracteres

    messages.add(new Message(currentUser, to, text, Date::now())); // el nuevo es el mas reciente, la lista sigue ordenada
    return MESSAGE_SENT;
}

// Metodo que llena res con la conversacion entre el usuario actual y other, y marca como leidos los que recibi
void UdeaBook::getConversation(User* other, DynamicList<Message*>& res){

    User* me = currentUser;

    for (int i = 0; i < messages.getSize(); i++) {
        Message* m = messages[i];

        bool sentByMe = (m->getSender() == me && m->getReceiver() == other);
        bool sentToMe = (m->getSender() == other && m->getReceiver() == me);

        if (sentByMe || sentToMe) {
            res.add(m); // salen en orden de fecha porque la lista ya esta ordenada
            if (sentToMe) m->markAsRead(); // si los estoy viendo, ya los lei
        }
    }
}

// Metodo que cuenta cuantos mensajes sin leer le ha enviado "from" al usuario actual
int UdeaBook::countUnreadFrom(User* from) const {

    int count = 0;
    for (int i = 0; i < messages.getSize(); i++) {
        const Message* m = messages[i];
        if (m->getSender() == from && m->getReceiver() == currentUser && !m->isRead()) count++;
    }
    return count;
}

// Metodo que cuenta todos los mensajes sin leer del usuario actual
int UdeaBook::countUnreadMessages() const {

    int count = 0;
    for (int i = 0; i < messages.getSize(); i++) {
        if (messages[i]->getReceiver() == currentUser && !messages[i]->isRead()) count++;
    }
    return count;
}
