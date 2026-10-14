#ifndef DYNAMICLIST_H
#define DYNAMICLIST_H
#include <stdexcept>

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

    T& operator[](int index); // Esta version de operator nos permite leer y modificar
    const T& operator[](int index) const; // Esta version de operator solo nos permite leer. (tiene const)

    bool isEmpty() const; // nos dice si la lista no tiene elementos. const porque solo consulta
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
    T* newData = new T[newCapacity]; // Es un puntero que apunta a un arreglo de elementos T. Si T es int, es un arreglo de int

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

// Operador [] para leer y modificar un elemento de la lista (ej: lista[2] = 99)
template <typename T>
T& DynamicList<T>::operator[](int index)
{
    if (index < 0 || index >= size) {
        throw std::out_of_range("DynamicList: index out of range"); // si el indice esta fuera de 0..size-1, detenemos el programa con un error claro
    }
    return data[index]; // devolvemos una referencia al elemento real, no una copia. Por eso se puede modificar
}

// Operador [] para solo leer un elemento. Se usa cuando la lista es const
template <typename T>
const T& DynamicList<T>::operator[](int index) const
{
    if (index < 0 || index >= size) {
        throw std::out_of_range("DynamicList: index out of range"); // misma validacion, las posiciones entre size y capacity tienen basura
    }
    return data[index]; // devolvemos una referencia const, se puede leer pero no modificar
}

// Metodo que nos dice si la lista esta vacia
template <typename T>
bool DynamicList<T>::isEmpty() const {
    return size == 0; // si no hay elementos guardados retorna true
}




#endif // DYNAMICLIST_H
