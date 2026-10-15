#include "photo.h"

// construcs
Photo::Photo() : path(""), size(0), likes(0){}

Photo::Photo(const string& path, unsigned long int size, unsigned int likes): path(path), size(size), likes(likes){}

Photo::Photo(const Photo& other): path(other.path), size(other.size), likes(other.likes){}

//methods

string Photo::getPath() const { return path; }

unsigned long int Photo::getSize() const {return size;}

unsigned int Photo::getLikes() const {return likes;}

// overloaded operators

Photo& Photo::operator++(){
    likes++;
    return *this;
}

Photo& Photo::operator--(){
    if (likes > 0) likes--;
    return *this;
}


