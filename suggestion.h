#ifndef SUGGESTION_H
#define SUGGESTION_H
#include <iostream>
using namespace std;
class User;

class Suggestion
{
public:
    Suggestion();
    Suggestion(User* user, int commonFriends);

    //getters
    User* getUser() const;
    int getCommonFriends() const;

    // overloaded operator
    bool operator<(const Suggestion& other) const;
    bool operator>(const Suggestion& other) const;
    friend ostream& operator<<(ostream& out, const Suggestion& suggestion);
private:
    User* user;
    int commonFriends;


};

#endif // SUGGESTION_H
