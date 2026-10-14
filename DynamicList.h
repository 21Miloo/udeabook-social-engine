#ifndef DYNAMICLIST_H
#define DYNAMICLIST_H

template <typename T> // <- Le dice a C++ que voy a crear una plantilla que puede trabajar con diferentes tipos de datos
class DynamicList{

private:
    T* data; // Arreglo dinamico donde viven los elementos
    int size; // cuantos elementos hay guardados
    int capacity; // cuantos caben antes de tener que crecer

    void resize(); // Lo hacemos privado. Solo la lista decide cuando crecer

public:
    DynamicList(int initialCapacity =10); // Constructor. por defecto se crea con una capacidad de 10 valores.
    ~DynamicList(); // Destructor

    void add(const T& element); // agrega al final
    int getSize() const; // cuantos elementos tiene. const es para asegurar que el metodo no modifica la lista, solo la consulta.


};

// Implementacion del Constructor
template <typename T>
DynamicList<T>:: DynamicList(int initialCapacity){

    capacity = (initialCapacity > 0) ? initialCapacity : 10;
    size = 0;
    data = new T[capacity];

}

// Implementacion del Destructor
template <typename T>
DynamicList<T>::~DynamicList(){
    delete[] data;
}

// Metodo para re-agrandar el arreglo
template <typename T>
void DynamicList<T>:: resize(){

    int newCapacity = capacity * 2; // Duplicamos la capacidad del arreglo
    T* newData = new T[newCapacity]; // arreglo de punteros con el tamaño nuevo

    for (int i = 0; i < size; ++i) {
        newData[i] = data[i];   // llenamos el nuevo arreglo con los datos del arreglo viejo
    }

    delete[] data; // eliminamos el arreglo data viejo para poderlo regenerarlo con el nuevo tamaño y asignarle los nuevos valores que contiene newData
    data = newData;
    capacity = newCapacity; // le asignamos a la capacidad la nueva capacidad. (Esto para consultarlo eventualmente)

}

// Metodo para agregar un elemento a la lista en la posicion size
template <typename T>
void DynamicList<T>:: add(const T& element){

    if (size == capacity){
        resize(); // Si el tamaño es igual a la capacidad, llamamos a la funcion resize para agrandar el arreglo
    }

    data[size] = element; // una vez agrandado el arreglo, escribimos el elemento en la posicion size
    size++;
}

// Metodo que nos retorna el tamaño del arreglo.
template <typename T>
int DynamicList<T>::getSize() const {
    return size;
}



#endif // DYNAMICLIST_H
