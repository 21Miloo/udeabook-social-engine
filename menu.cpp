#include "menu.h"
#include <iostream>

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
        cin>>option;
        switch (option) {
        case 1:cout<<"Administrar amistades"<<endl;
            break;
        case 2:cout<<"Visitar perfil"<<endl;
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
