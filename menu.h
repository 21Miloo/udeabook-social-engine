#ifndef MENU_H
#define MENU_H

class Album;

class Menu
{
public:
    Menu();
    void run();

private:
    int readOption(int min, int max);
    void visitProfileMenu();
    void showCarousel(Album* album);
};

#endif // MENU_H
