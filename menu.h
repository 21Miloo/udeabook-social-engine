#ifndef MENU_H
#define MENU_H

class Menu
{
public:
    Menu();
    void run();

private:
    int readOption(int min, int max);
    void visitProfileMenu();
};

#endif // MENU_H
