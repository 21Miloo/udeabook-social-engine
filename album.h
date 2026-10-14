#ifndef ALBUM_H
#define ALBUM_H
#include <string>
#include "privacy.h"
#include "DynamicList.h"
#include "photo.h"
using namespace std;
class User;

class Album
{
public:
    Album();
    Album(const string& name, const string& path, Privacy privacy, User* owner);

private:
    string name;
    string path;
    Privacy privacy;    
    DynamicList<Photo*> photos;
    User *owner;
};

#endif // ALBUM_H
