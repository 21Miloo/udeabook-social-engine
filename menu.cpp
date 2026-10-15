#include "menu.h"
#include "album.h"
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
    string userName;
    cout<<"Nombre de usuario: ";
    cin>>userName;
    cout<<"Visitando perfil de "<<userName<<endl;

    // TODO: buscar el usuario en la plataforma y verificar que sea amigo (cuando exista User)
    Album a1("Cartagena2026", "/users/x/" + userName + "/albums/Cartagena2026", PUBLIC, nullptr);
    Album a2("Familia", "/users/x/" + userName + "/albums/Familia", FRIENDS, nullptr);
    Album a3("Privado", "/users/x/" + userName + "/albums/Privado", PRIVATE, nullptr);

    DynamicList<Album*> albums;
    albums.add(&a1);
    albums.add(&a2);
    albums.add(&a3);

    DynamicList<Album*> visibles;

    for(int i = 0; i < albums.getSize(); i++){
        if(albums[i]->isVisibleTo(false, true)) visibles.add(albums[i]);
    }

    if(visibles.getSize() == 0){
        cout<<"Este usuario no tiene albumes visibles"<<endl;
        return;
    }

    cout<<"Albumes de "<<userName<<":"<<endl;
    for(int i = 0; i < visibles.getSize(); i++){
        cout<<i + 1<<". "<<visibles[i]->getName()<<" ("<<visibles[i]->photoCount()<<" fotos)"<<endl;
    }
    cout<<"0. Volver"<<endl;
    cout<<"> ";
    int op = readOption(0, visibles.getSize());
    if(op == 0) return;

    Album* chosen = visibles[op - 1];
    cout<<"Abriendo album "<<chosen->getName()<<endl;
}
