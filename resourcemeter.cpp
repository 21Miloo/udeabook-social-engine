#include "resourcemeter.h"
#include <iostream>

unsigned long long int ResourceMeter::iterations = 0;
int ResourceMeter::externalComponents = 0;

void ResourceMeter::reset()
{
    iterations = 0;
    externalComponents = 0;
}

void ResourceMeter::add(unsigned long long int n){
    iterations = iterations + n;
}

void ResourceMeter::registerExternal(const string& name){
    externalComponents = externalComponents + 1;
    cout<<"Componente externo usado: "<<name<<endl;
}

unsigned long long int ResourceMeter::getIterations()
{
    return iterations;
}

void ResourceMeter::report(size_t memoryBytes){
    cout << "--- Consumo de recursos ---" << endl;
    cout << "Iteraciones: " << iterations << endl;
    cout << "Componentes externos: " << externalComponents << endl;
    cout << "Memoria usada: " << memoryBytes << " bytes" << endl;
}
