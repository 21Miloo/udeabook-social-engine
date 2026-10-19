#include "menu.h"
#include "album.h"
#include "resourcemeter.h"
#include <iostream>
#include <string>
#include "udeabook.h"

using namespace std;

Menu::Menu(UdeaBook& platform):platform(platform){}

void Menu::run(){
    short int option;
    if (!loginScreen()) return;
    showBestFriends();
    do{
        ResourceMeter::add();
        cout<<endl<<"==== Bienvenido a UdeaBook ===="<<endl;
        cout<<"1. Administrar amistades"<<endl;
        cout<<"2. Visitar perfil de un amigo"<<endl;
        cout<<"3. Ver sugerencias de amistad"<<endl;
        cout<<"4. Administrar mensajes"<<endl;
        cout<<"0. Salir"<<endl;
        cout<<"> ";
        option = readOption(0, 4);
        switch (option) {
        case 1:manageFriendshipsMenu();
            break;
        case 2:visitProfileMenu();
            break;
        case 3:suggestionsMenu();
            break;
        case 4:messagesMenu();
            break;
        case 0:cout<<"Hasta luego"<<endl;
            break;
        default:cout<<"Opcion invalida"<<endl;
            break;
        }
    }while(option != 0);
}

int Menu::readOption(int min, int max){
    int num;
    while(true){
        ResourceMeter::add();
        cin>>num;
        if(cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            cout<<"Debe ingresar un numero: ";
        }
        else if(num < min || num > max){
            cout<<"Opcion fuera de rango ("<<min<<"-"<<max<<"): ";
        }
        else{
            return num;
        }
    }
}

void Menu::visitProfileMenu(){
    ResourceMeter::reset();
    string userName;
    cout<<"Nombre de usuario: ";
    cin>>userName;
    cout<<"Visitando perfil de "<<userName<<endl;


    Album a1("Antioquia", "/users/x/" + userName + "/albums/Antioquia", PUBLIC, nullptr);
    Album a2("Paseos", "/users/x/" + userName + "/albums/Paseos", FRIENDS, nullptr);
    Album a3("Privado", "/users/x/" + userName + "/albums/Privado", PRIVATE, nullptr);

    a1.addPhoto(new Photo(a1.getPath() + "/Guatape.png", Date(12, 1, 2026), 2048000, 12));
    a1.addPhoto(new Photo(a1.getPath() + "/PiedraDelPenol.png", Date(12, 1, 2026), 1536000, 4));
    a1.addPhoto(new Photo(a1.getPath() + "/SantaFe.png", Date(3, 4, 2026), 2870000, 25));
    a1.addPhoto(new Photo(a1.getPath() + "/Jardin.png", Date(20, 6, 2026), 980000, 7));
    a1.addPhoto(new Photo(a1.getPath() + "/ParqueArvi.png", Date(15, 8, 2026), 3410000));

    a2.addPhoto(new Photo(a2.getPath() + "/AltoDelChocho.png", Date(7, 2, 2026), 3100000, 10));
    a2.addPhoto(new Photo(a2.getPath() + "/Itagui.png", Date(14, 3, 2026), 2250000, 6));
    a2.addPhoto(new Photo(a2.getPath() + "/Bogota.png", Date(30, 5, 2026), 1780000, 3));
    a2.addPhoto(new Photo(a2.getPath() + "/Envigado.png", Date(19, 7, 2026), 4020000, 18));
    a2.addPhoto(new Photo(a2.getPath() + "/SanAntonioDePrado.png", Date(2, 9, 2026), 640000, 9));

    DynamicList<Album*> albums;
    albums.add(&a1);
    albums.add(&a2);
    albums.add(&a3);

    DynamicList<Album*> visibles;

    for(int i = 0; i < albums.getSize(); i++){
        ResourceMeter::add();
        if(albums[i]->isVisibleTo(false, true)) visibles.add(albums[i]);
    }

    int option = 0;
    if(visibles.getSize() == 0){
        cout<<"Este usuario no tiene albumes visibles"<<endl;
    }
    else{
        do{
            ResourceMeter::add();
            cout<<"Albumes de "<<userName<<":"<<endl;
            for(int i = 0; i < visibles.getSize(); i++){
                ResourceMeter::add();
                cout<<i + 1<<". "<<visibles[i]->getName()<<" ("<<visibles[i]->photoCount()<<" fotos)"<<endl;
            }
            cout<<"0. Volver"<<endl;
            cout<<"> ";
            option = readOption(0, visibles.getSize());
            if(option != 0) showCarousel(visibles[option - 1]);
        }while(option != 0);
    }


    reportResources(sizeof(userName) + userName.capacity() + sizeof(option)
                    + a1.calculateMemory() + a2.calculateMemory() + a3.calculateMemory()
                    + albums.calculateMemory() + visibles.calculateMemory());
}

void Menu::manageFriendshipsMenu(){
    int option;
    do{
        ResourceMeter::add();
        cout<<endl<<"==== Administrar amistades ===="<<endl;
        cout<<"1. Enviar solicitud de amistad"<<endl;
        cout<<"2. Ver y aceptar solicitudes recibidas"<<endl;
        cout<<"0. Volver"<<endl;
        cout<<"> ";
        option = readOption(0, 2);
        switch (option) {
        case 1:sendRequestMenu();
            break;
        case 2:acceptRequestsMenu();
            break;
        case 0:cout<<"Volviendo al menu principal"<<endl;
            break;
        }
    }while(option != 0);
}

void Menu::showCarousel(Album* album){
    if(album == nullptr) return;
    cout<<"Abriendo album "<<album->getName()<<endl;

    if (album->photoCount() == 0) {
        cout << "Este album no tiene fotos." << endl;
        return;
    }

    short int photoSelected = 0;
    int total = album->photoCount();
    int option;
    do{
        ResourceMeter::add();
        cout << "[Foto "<<photoSelected+1<<"/"<<total<<"]" << endl;
        cout <<album->getPhoto(photoSelected)->getPath() << endl;

        cout << "1. Foto anterior" << endl;
        cout << "2. Siguiente foto" << endl;
        cout << "3. Dar like" << endl;
        cout << "4. Quitar like" << endl;
        cout << "0. Volver" << endl;

        cout << "> ";
        option = readOption(0, 4);

        switch (option) {
            case 1:
                photoSelected = (photoSelected - 1 + total) % total;
                break;
            case 2:
                photoSelected = (photoSelected + 1) % total;
                break;
            case 3:
                ++(*album->getPhoto(photoSelected));
                cout << "Foto ahora tiene " << album->getPhoto(photoSelected)->getLikes() << " likes." << endl;
                break;
            case 4:
                --(*album->getPhoto(photoSelected));
                cout << "Foto ahora tiene " << album->getPhoto(photoSelected)->getLikes() << " likes." << endl;
                break;
            case 0:
                cout << "Volviendo al menu de albumes." << endl;
                break;
            default:
                cout << "Opcion invalida." << endl;
        }
    }while(option != 0);
}

bool Menu::loginScreen(){
    string userName;
    string password;
    cout<<endl<<"==== Iniciar sesion ===="<<endl;
    cout<<"Usuario: ";
    cin>>userName;
    cout<<"Contrasena: ";
    cin>>password;

    if(platform.login(userName, password)){
        cout<<"Bienvenido, "<<platform.getCurrentUser()->getUserName()<<endl;
        return true;
    }
    cout<<"Usuario o contrasena incorrectos"<<endl;
    return false;
}


void Menu::sendRequestMenu(){
    ResourceMeter::reset();
    string userName;
    cout<<"Nombre del usuario: ";
    cin>>userName;

    User* user = platform.findUser(userName);
    if(user == nullptr){
        cout<<"Ese usuario no existe"<<endl;
    }
    else{
        printRequestResult(platform.sendRequest(user), user);
    }

    reportResources(sizeof(userName) + userName.capacity() + sizeof(user));
}

void Menu::acceptRequestsMenu(){
    ResourceMeter::reset();
    const DynamicList<User*>& requests = platform.getCurrentUser()->getReceivedRequests();
    int selected = 0;
    int action = 0;

    if(requests.getSize() == 0){
        cout<<"No tienes solicitudes pendientes"<<endl;
    }
    else{
        cout<<"Solicitudes recibidas:"<<endl;
        for(int i = 0; i < requests.getSize(); i++){
            ResourceMeter::add();
            cout<<i + 1<<". "<<*requests[i]<<endl;
        }
        cout<<"0. Volver"<<endl;
        cout<<"> ";
        selected = readOption(0, requests.getSize());
    }

    if(selected != 0){
        User* sender = requests[selected - 1];

        cout<<"1. Aceptar"<<endl;
        cout<<"2. Rechazar"<<endl;
        cout<<"0. Cancelar"<<endl;
        cout<<"> ";
        action = readOption(0, 2);

        if(action == 1){
            platform.acceptRequest(sender);
            cout<<"Ahora eres amigo de "<<sender->getUserName()<<endl;
        }
        else if(action == 2){
            platform.rejectRequest(sender);
            cout<<"Solicitud de "<<sender->getUserName()<<" rechazada"<<endl;
        }
    }

    reportResources(sizeof(&requests) + sizeof(selected) + sizeof(action) + sizeof(User*));
}

void Menu::printRequestResult(RequestResult result, User* user){
    switch(result){
    case REQUEST_SENT:
        cout<<"Solicitud enviada a "<<user->getUserName()<<endl;
        break;
    case REQUEST_TO_MYSELF:
        cout<<"No te puedes enviar una solicitud a ti mismo"<<endl;
        break;
    case REQUEST_ALREADY_FRIENDS:
        cout<<"Ya eres amigo de "<<user->getUserName()<<endl;
        break;
    case REQUEST_ALREADY_SENT:
        cout<<"Ya le habias enviado una solicitud"<<endl;
        break;
    case REQUEST_PENDING_FROM_THEM:
        cout<<user->getUserName()<<" ya te envio una solicitud, aceptala en Administrar amistades"<<endl;
        break;
    }
}

void Menu::suggestionsMenu(){
    ResourceMeter::reset();
    cout<<"Cuantas sugerencias quieres ver (1-20): ";
    int amount = readOption(1, 20);

    DynamicList<Suggestion> suggestions;
    platform.getSuggestions(amount, suggestions);
    int selected = 0;

    if(suggestions.getSize() == 0){
        cout<<"No hay sugerencias por ahora"<<endl;
    }
    else{
        cout<<"Sugerencias de amistad:"<<endl;
        for(int i = 0; i < suggestions.getSize(); i++){
            ResourceMeter::add();
            cout<<i + 1<<". "<<suggestions[i]<<endl;
        }
        cout<<"Enviar solicitud a (0 para volver): ";
        selected = readOption(0, suggestions.getSize());
    }

    if(selected != 0){
        User* user = suggestions[selected - 1].getUser();
        printRequestResult(platform.sendRequest(user), user);
    }

    reportResources(sizeof(amount) + sizeof(selected) + sizeof(User*) + suggestions.calculateMemory());
}

void Menu::showBestFriends(){
    ResourceMeter::registerExternal("mt19937 de <random> (seleccion aleatoria de amigos)");
    DynamicList<User*> bestFriends;
    platform.getBestFriends(bestFriends);
    DynamicList<User*> newBestFriends;

    if(bestFriends.getSize() == 0){
        cout<<"Todavia no tienes amigos para mostrar"<<endl;
    }
    else{
        cout<<endl<<"==== Tus mejores amigos ===="<<endl;
        for(int i = 0; i < bestFriends.getSize(); i++){
            ResourceMeter::add();
            cout<<bestFriends[i]->getUserName()<<": "<<bestFriends[i]->getProfilePhotoPath()<<endl;
        }

        cout<<"1. Ver otros amigos"<<endl;
        cout<<"0. Continuar"<<endl;
        cout<<"> ";
        if(readOption(0, 1) == 1 && platform.updateBestFriends(newBestFriends)){
            cout<<endl<<"==== Tus mejores amigos ===="<<endl;
            for(int i = 0; i < newBestFriends.getSize(); i++){
                ResourceMeter::add();
                cout<<newBestFriends[i]->getUserName()<<": "<<newBestFriends[i]->getProfilePhotoPath()<<endl;
            }
        }
    }

    reportResources(bestFriends.calculateMemory() + newBestFriends.calculateMemory());
}

void Menu::messagesMenu(){
    int option;
    do{
        ResourceMeter::add();
        cout<<endl<<"==== Administrar mensajes ===="<<endl;
        cout<<"1. Enviar mensaje"<<endl;
        cout<<"2. Leer nuevos mensajes ("<<platform.countUnreadMessages()<<")"<<endl;
        cout<<"3. Buscar mensajes por rango de fechas"<<endl;
        cout<<"0. Volver"<<endl;
        cout<<"> ";
        option = readOption(0, 3);
        switch (option) {
        case 1:sendMessageMenu();
            break;
        case 2:readNewMessagesMenu();
            break;
        case 3:searchMessagesMenu();
            break;
        case 0:cout<<"Volviendo al menu principal"<<endl;
            break;
        }
    }while(option != 0);
}

void Menu::sendMessageMenu(){
    ResourceMeter::reset();
    string friendName;
    string text;
    cout<<"Nombre del amigo: ";
    cin>>friendName;

    User* receiver = platform.findUser(friendName);
    if(receiver == nullptr){
        cout<<"Ese usuario no existe"<<endl;
    }
    else{
        cout<<"Mensaje (maximo 512 caracteres): ";
        cin.ignore(1000, '\n');
        getline(cin, text);

        ResourceMeter::registerExternal("chrono (fecha y hora actual del mensaje)");
        switch(platform.sendMessage(receiver, text)){
        case MESSAGE_SENT:
            cout<<"Mensaje enviado a "<<receiver->getUserName()<<endl;
            break;
        case MESSAGE_NOT_FRIENDS:
            cout<<"Solo puedes enviar mensajes a tus amigos"<<endl;
            break;
        case MESSAGE_INVALID_TEXT:
            cout<<"El mensaje no puede estar vacio ni pasar de 512 caracteres"<<endl;
            break;
        }
    }

    reportResources(sizeof(friendName) + friendName.capacity() + sizeof(text) + text.capacity() + sizeof(receiver));
}

void Menu::readNewMessagesMenu(){
    ResourceMeter::reset();
    DynamicList<Message*> newMessages;
    platform.readNewMessages(newMessages);

    if(newMessages.getSize() == 0){
        cout<<"No tienes mensajes nuevos"<<endl;
    }
    else{
        cout<<"Mensajes nuevos:"<<endl;
        for(int i = 0; i < newMessages.getSize(); i++){
            ResourceMeter::add();
            cout<<*newMessages[i]<<endl;
        }
    }

    reportResources(newMessages.calculateMemory());
}

void Menu::searchMessagesMenu(){
    ResourceMeter::reset();
    Date start;
    Date end;
    DynamicList<Message*> found;

    if(!readDate("Fecha de inicio (aaaa-mm-dd): ", false, start) || !readDate("Fecha de fin (aaaa-mm-dd): ", true, end)){
        // readDate ya mostro el error
    }
    else if(end < start){
        cout<<"La fecha de fin no puede ser anterior a la de inicio"<<endl;
    }
    else{
        platform.searchMessagesByRange(start, end, found);

        if(found.getSize() == 0){
            cout<<"No recibiste mensajes en ese rango"<<endl;
        }
        else{
            cout<<"Mensajes recibidos en el rango:"<<endl;
            for(int i = 0; i < found.getSize(); i++){
                ResourceMeter::add();
                cout<<*found[i]<<endl;
            }
        }
    }

    reportResources(sizeof(start) + sizeof(end) + found.calculateMemory());
}

// Lee una fecha aaaa-mm-dd. Si endOfDay es true queda a las 23:59 para que el dia de fin se incluya completo
bool Menu::readDate(const string& text, bool endOfDay, Date& date){
    string input;
    cout<<text;
    cin>>input;

    bool valid = input.length() == 10 && input[4] == '-' && input[7] == '-';
    for(int i = 0; i < 10 && valid; i++){
        ResourceMeter::add();
        if(i != 4 && i != 7 && (input[i] < '0' || input[i] > '9')) valid = false;
    }
    if(!valid){
        cout<<"Fecha invalida, use el formato aaaa-mm-dd"<<endl;
        return false;
    }

    int year = (input[0] - '0') * 1000 + (input[1] - '0') * 100 + (input[2] - '0') * 10 + (input[3] - '0');
    int month = (input[5] - '0') * 10 + (input[6] - '0');
    int day = (input[8] - '0') * 10 + (input[9] - '0');
    if(month < 1 || month > 12 || day < 1 || day > 31){
        cout<<"Fecha invalida"<<endl;
        return false;
    }

    if(endOfDay) date = Date(day, month, year, 23, 59);
    else date = Date(day, month, year, 0, 0);
    return true;
}

// Muestra las metricas de la funcionalidad que acaba de terminar.
// La memoria es la de TODA la plataforma en este momento + el menu + las variables locales de la funcionalidad
void Menu::reportResources(size_t localMemory){
    ResourceMeter::report(platform.calculateTotalMemory() + sizeof(*this) + localMemory);
}
