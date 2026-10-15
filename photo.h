#ifndef PHOTO_H
#define PHOTO_H
#include <string>

using namespace std;

class Photo
{
public:
    Photo();
    Photo(const string& path, unsigned long int size, unsigned int likes = 0);
    Photo(const Photo& other);  //copy constructor

    string getPath() const;
    unsigned long int getSize() const;
    unsigned int getLikes() const;
    size_t calculateMemory() const;
    Photo& operator++();
    Photo& operator--();

private:
    string path;
    // Date captureDate;
    unsigned long int size;
    unsigned int likes;
};

#endif // PHOTO_H
