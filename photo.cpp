#include "photo.h"

Photo::Photo() : path(""), size(0), likes(0){}

Photo::Photo(const string& path, unsigned long int size, unsigned int likes): path(path), size(size), likes(likes){}

Photo::Photo(const Photo& other): path(other.path), size(other.size), likes(other.likes){}
