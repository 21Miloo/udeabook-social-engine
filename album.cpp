#include "album.h"
#include "privacy.h"

Album::Album() : name(""), path(""), privacy(PRIVATE), owner(nullptr) {}

Album::Album(const string& name, const string& path, Privacy privacy, User* owner): name(name), path(path), privacy(privacy), owner(owner){}
