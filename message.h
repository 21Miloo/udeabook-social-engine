#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include "date.h"
class User;

using namespace std;

class Message
{
public:
    Message();
    Message(User* sender,User* receiver, const string& text, const Date& sentDate);
    //getters
    User* getSender() const;
    User* getReceiver() const;
    const string& getText() const;
    const Date& getSentDate() const;
    bool isRead() const;

    //setters
    void markAsRead();
    static bool isValidText(const string& text);

    // overload operator
    bool operator<(const Message& other) const;

    //memory
    size_t calculateMemory() const;

    //friend
    friend ostream& operator<<(ostream& out, const Message& m);

private:
    User* sender;
    User* receiver;
    string text;
    Date sentDate;
    bool read;
    static const int MAX_CHARACTERS = 512;

};

#endif // MESSAGE_H
