#include "menu.h"
#include "album.h"
#include "resourcemeter.h"
#include <iostream>
#include <string>

using namespace std;

Menu::Menu() {}

void Menu::run(){
    short int option;
    do{
        cout<<endl<<"==== Bienvenido a UdeaBook ===="<<endl;
        cout<<"1. Administrar amistades"<<endl;
        cout<<"2. Visitar perfil de un amigo"<<endl;
        cout<<"3. Ver sugerencias de amistad"<<endl;
        cout<<"4. Administrar mensajes"<<endl;
        cout<<"0. Salir"<<endl;
        cout<<"> ";
        option = readOption(0, 4);
        switch (option) {
        case 1:cout<<"Administrar amistades"<<endl;
            break;
        case 2:visitProfileMenu();
            break;
        case 3:cout<<"Sugerencias"<<endl;
            break;
        case 4:cout<<"Mensajes"<<endl;
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


    size_t memory = sizeof(*this) + sizeof(userName) + userName.capacity() + sizeof(option)
                    + a1.calculateMemory() + a2.calculateMemory() + a3.calculateMemory()
                    + albums.calculateMemory() + visibles.calculateMemory();
    ResourceMeter::report(memory);
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
