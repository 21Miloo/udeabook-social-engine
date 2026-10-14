#ifndef PHOTO_H
#define PHOTO_H
#include <string>

using namespace std;

class Photo
{
public:
    Photo();

private:
    string path;
    // Date captureDate;
    unsigned long int size;
    unsigned int likes;
};

#endif // PHOTO_H
