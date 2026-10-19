#include <iostream>
#include "menu.h"
#include "resourcemeter.h"
 #include "udeabook.h"
using namespace std;

int main()
{
    UdeaBook platform("data/", "/users/udeabook");

    ResourceMeter::reset(); // la medicion empieza con la carga de datos
    if (!platform.loadData()) {
        cout << "No se pudieron cargar los datos" << endl;
        return 1;
    }

    Menu menu(platform);

    menu.run();

    if (!platform.saveData()) {
        cout << "Algunos cambios no se pudieron guardar" << endl;
    }

    return 0;
}
