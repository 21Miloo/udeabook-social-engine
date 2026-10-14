#ifndef PHOTO_H
#define PHOTO_H
#include <string>

using namespace std;

class Photo
{
public:
    Photo();
    Photo(const string& path, unsigned long int size, unsigned int likes = 0);
    Photo(const Photo& other); //constructor of copy

    string getPath() const;
    unsigned long int getSize() const;
    unsigned int getLikes() const;
    void operator++();
    void operator--();
private:
    string path;
    // Date captureDate;
    unsigned long int size;
    unsigned int likes;
};

#endif // PHOTO_H
