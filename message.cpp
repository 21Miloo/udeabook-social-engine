#include "message.h"
#include "user.h"

//constructor by default
Message::Message():sender(nullptr), receiver(nullptr), text(""), sentDate(),read(false) {}

Message::Message(User* sender,User* receiver,const string& text,const Date& sentDate){
    this->sender = sender;
    this->receiver = receiver;
    this->text = text;
    this->sentDate = sentDate;
    this-> read = false;
}


// getters

User* Message::getSender() const {
    return this->sender;
}

User* Message::getReceiver() const{
    return this->receiver;
}
const string& Message::getText() const{
    return this->text;
}
const Date& Message::getSentDate() const{
    return this->sentDate;
}
bool Message::isRead() const{
    return this->read;
}

//setters

void Message::markAsRead(){
    this->read = true;
}

bool Message::isValidText(const string& text){
    if(text.length()>MAX_CHARACTERS || text.length() == 0 ) return false;

    return true;
}

//sobrecarga
bool Message::operator<(const Message& other) const {
    return this->sentDate < other.sentDate;
}

//memory

size_t Message::calculateMemory() const {
    return sizeof(*this) + text.capacity();
}

ostream& operator<<(ostream& out, const Message& m) {
    out << "[" << m.sentDate << "] ";
    if (m.sender != nullptr) out << m.sender->getUserName();
    else out << "(desconocido)";
    out << ": " << m.text;
    return out;
}
