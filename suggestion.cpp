#include "suggestion.h"
#include "user.h"
Suggestion::Suggestion(): user(nullptr), commonFriends(0) {}
Suggestion::Suggestion(User* user, int commonFriends){
    this->user = user;
    this->commonFriends = commonFriends;
}


User* Suggestion::getUser() const{
    return this->user;
}
int Suggestion::getCommonFriends() const{
    return this->commonFriends;
}

bool Suggestion::operator<(const Suggestion& other) const{
    return this->commonFriends < other.commonFriends;
}
bool Suggestion::operator>(const Suggestion& other) const{
    return this->commonFriends > other.commonFriends;
}
//friend
ostream& operator<<(ostream& out, const Suggestion& suggestion){
    if (suggestion.user != nullptr) out << suggestion.user->getUserName();
    else out << "(desconocido)";
    out << " (" << suggestion.commonFriends << " amigos en comun)";
    return out;
}

size_t Suggestion::calculateMemory() const{
    return sizeof(*this); // el usuario al que apunta no es de la sugerencia
}
