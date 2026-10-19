#include <iostream>
#include "menu.h"
 #include "udeabook.h"
using namespace std;

int main()
{
    UdeaBook platform("data/", "/users/udeabook");

    if (!platform.loadData()) {
        cout << "No se pudieron cargar los datos" << endl;
        return 1;
    }

    Menu menu(platform);

    menu.run();

    return 0;
}
