#ifndef DYNAMICLIST_H
#define DYNAMICLIST_H

template <typename T> // <- Le dice a C++ que voy a crear una plantilla que puede trabajar con diferentes tipos de datos
class DynamicList{

private:
    T* data; // Arreglo dinamico donde viven los elementos
    int size; // cuantos elementos hay guardados
    int capacity; // cuantos caben antes de tener que crecer


public:
    DynamicList(int initialCapacity =10); // Constructor. por defecto se crea con una capacidad de 10 valores.
    ~DynamicList(); // Destructor

};

template <typename T>
DynamicList<T>:: DynamicList(int initialCapacity){

    capacity = (initialCapacity > 0) ? initialCapacity : 10;
    size = 0;
    data = new T(capacity);

};

template <typename T>
DynamicList<T>::~DynamicList(){
    delete[] data;
}



#endif // DYNAMICLIST_H
