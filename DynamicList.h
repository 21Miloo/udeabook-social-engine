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

    DynamicList(const DynamicList<T>& other); // Constructor de copia
    DynamicList<T> &operator=(const DynamicList<T>& other); // Asignacion - Operator

    ~DynamicList(); // Destructor

    void add(const T& element); // agrega al final

    void clear(); // vacia la lista

    int getSize() const; // cuantos elementos tiene. const es para asegurar que el metodo no modifica la lista, solo la consulta.

    T& operator[](int index); // Esta version de operator nos permite leer y modificar
    const T& operator[](int index) const; // Esta version de operator solo nos permite leer. (tiene const)

    bool isEmpty() const; // nos dice si la lista no tiene elementos. const porque solo consulta

    void removeAt(int index); // Quita un elemento de una posicion (cuando un usuario acepta o rechaza una solicitud de amistad, hay que quitarla de su lista de solicitudes pendientes.)

    void insertAt(int index, const T& element); // pone el elemento en una posicion especifica

    int indexOf(const T& element) const; // dice en qué posición está un elemento, o -1 si no esta

    bool contains(const T& element) const; // dice si está o no (true/false)

    int calculateMemory() const; // cuantos bytes ocupa la lista en memoria (para la medicion de recursos)
};

// Implementacion del Constructor
template <typename T>
DynamicList<T>:: DynamicList(int initialCapacity){

    capacity = (initialCapacity > 0) ? initialCapacity : 10;
    size = 0;
    data = new T[capacity];

}

// Implementacion del Constructor de Copia
template <typename T>
DynamicList<T>:: DynamicList(const DynamicList<T>& other){
    capacity = other.capacity;
    size = other.size;
    data = new T[capacity];

    for (int i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }

}

// Implementacion del Operator=
template <typename T>
DynamicList<T> &DynamicList<T>:: operator=(const DynamicList<T>& other){

    if(this == &other){
        return *this;
    }

    delete[] data;
    capacity = other.capacity;
    size = other.size;
    data = new T[capacity];

    for (int i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }

    return *this;
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

// Metodo que remueve un elemento de la lista
template <typename T>
void DynamicList<T>::removeAt(int index){

    if (index < 0 || index >= size) {
        throw std::out_of_range("DynamicList: index out of range"); // solo se puede quitar algo que existe (0..size-1)
    }

    for (int i = index; i < size - 1; i++) {
        data[i] = data[i + 1]; // corremos los elementos a la izquierda, el que se quita queda pisado por el siguiente
    }
    size--; // la ultima casilla queda como basura, ya no se puede acceder a ella
}

// Metodo que inserta un elemento en una posicion especifica
template <typename T>
void DynamicList<T>::insertAt(int index, const T& element)
{
    if (index < 0 || index > size) {
        throw std::out_of_range("DynamicList: index out of range"); // aqui index == size si es valido, significa insertar al final
    }

    if (size == capacity) {
        resize(); // si esta lleno agrandamos antes de mover, porque vamos a escribir en data[size]
    }

    for (int i = size; i > index; i--) {
        data[i] = data[i - 1]; // corremos a la derecha empezando por el final para no pisar datos antes de moverlos
    }

    data[index] = element; // ya quedo libre la posicion index, escribimos el elemento
    size++;
}

// Metodo que busca un elemento y retorna su posicion
template <typename T>
int DynamicList<T>:: indexOf(const T& element) const {

    for (int i = 0; i < size; ++i) {
        if (data[i] == element) return i; // apenas lo encontramos retornamos, no tiene sentido seguir buscando
    }

    return -1; // no esta en la lista. -1 nunca es una posicion valida
}

// Metodo que nos dice si un elemento esta en la lista
template <typename T>
bool DynamicList<T>::contains(const T& element) const
{
    int index = indexOf(element); // reutilizamos indexOf para no escribir la busqueda dos veces

    if (index != -1) {
        return true;
    }

    return false;
}

// Metodo para vaciar la lista
template <typename T>
void DynamicList<T>:: clear(){
    size = 0; // no liberamos data, la lista debe seguir siendo usable y conserva su capacidad
}


// Metodo que calcula cuantos bytes ocupa la lista
template <typename T>
int DynamicList<T>::calculateMemory() const {
    return sizeof(*this) + capacity * sizeof(T);
}


#endif // DYNAMICLIST_H
