#include "photo.h"

// constructors
Photo::Photo() : path(""), size(0), likes(0){}

Photo::Photo(const string& path, const Date& captureDate, unsigned long int size, unsigned int likes): path(path), captureDate(captureDate), size(size), likes(likes){}

Photo::Photo(const Photo& other): path(other.path), captureDate(other.captureDate), size(other.size), likes(other.likes){}

//methods

string Photo::getPath() const { return path; }

Date Photo::getCaptureDate() const { return captureDate; }

unsigned long int Photo::getSize() const {return size;}

unsigned int Photo::getLikes() const {return likes;}

size_t Photo::calculateMemory() const
{
    return sizeof(*this) + path.capacity();
}

// overloaded operators

Photo& Photo::operator++(){
    likes++;
    return *this;
}

Photo& Photo::operator--(){
    if (likes > 0) likes--;
    return *this;
}


