#ifndef RESOURCEMETER_H
#define RESOURCEMETER_H
#include <string>

using namespace std;

class ResourceMeter
{
public:
    static void reset();
    static void add(unsigned long long int n = 1);
    static void registerExternal(const string& name);
    static unsigned long long int getIterations();
    static void report(size_t memoryBytes);

private:    
    static unsigned long long int iterations;
    static int externalComponents;

};

#endif // RESOURCEMETER_H
