#ifndef MENU_H
#define MENU_H

#include "udeabook.h"

class Album;
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
    void showBestFriends();
    void sendRequestMenu();
    void acceptRequestsMenu();
    void suggestionsMenu();
    void printRequestResult(RequestResult result, User* user);
    void messagesMenu();
    void sendMessageMenu();
    void readNewMessagesMenu();
    void searchMessagesMenu();
    bool readDate(const string& text, bool endOfDay, Date& date);
    void reportResources(size_t localMemory);
};

#endif // MENU_H
