#ifndef MENU_H
#define MENU_H

class Album;
class UdeaBook;
class Menu
{
public:
    Menu(UdeaBook& platform);
    void run();

private:
    int readOption(int min, int max);
    void visitProfileMenu();
    void manageFriendshipsMenu();
    void showCarousel(Album* album);
    UdeaBook& platform;
    bool loginScreen();
    void sendRequestMenu();
    void acceptRequestsMenu();
};

#endif // MENU_H
