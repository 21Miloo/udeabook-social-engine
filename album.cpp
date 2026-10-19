#include "album.h"
#include "resourcemeter.h"

Album::Album() : name(""), path(""), privacy(PRIVATE), owner(nullptr) {}

Album::Album(const string& name, const string& path, Privacy privacy, User* owner): name(name), path(path), privacy(privacy), owner(owner){}

Album::Album(const Album& other)
    : name(other.name), path(other.path), privacy(other.privacy), owner(other.owner)
{
    for(int i = 0; i < other.photos.getSize(); i++){
        photos.add(new Photo(*other.photos[i]));
    }
}

Album& Album::operator=(const Album& other){
    if(this == &other) return *this;

    for(int i = 0; i < photos.getSize(); i++){
        delete photos[i];
    }
    photos.clear();

    name = other.name;
    path = other.path;
    privacy = other.privacy;
    owner = other.owner;
    for(int i = 0; i < other.photos.getSize(); i++){
        photos.add(new Photo(*other.photos[i]));
    }
    return *this;
}

//destructor
Album::~Album() {
    for(int i=0; i<photos.getSize();i++){
        delete photos[i];
    }
}

void Album::addPhoto(Photo* photo){

    if (photo == nullptr) return;

    photos.add(photo);
}

string Album::getName() const{
    return name;
}
string Album::getPath() const{
    return path;
}
Privacy Album::getPrivacy() const{
    return privacy;
}

int Album::photoCount() const{
    return photos.getSize(); // photos is private, so this public method lets code outside the class get the number of photos
}

User* Album::getOwner() const{
    return owner;
}

Photo* Album::getPhoto(int i) const{
    if(i < 0 ||i >= photoCount()) return nullptr;
    return photos[i];
}

bool Album::isVisibleTo(bool isOwner, bool isFriend) const{
    if(isOwner || privacy == PUBLIC || (privacy == FRIENDS && isFriend)) return true;

    return false;
}

size_t Album::calculateMemory() const
{
    size_t memory = sizeof(*this) + name.capacity() + path.capacity() + photos.calculateMemory() - sizeof(photos);
    for(int i = 0; i < photos.getSize(); i++){
        memory = memory + photos[i]->calculateMemory();
    }
    return memory;
}
