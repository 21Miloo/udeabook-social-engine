#include "adjacencymatrix.h"

// Implementacion del Constructor
AdjacencyMatrix::AdjacencyMatrix(int capacity){

    this->capacity = (capacity > 0) ? capacity : 10; // this->capacity es el atributo, capacity solo es el parametro
    n = 0;

    matrix = new bool*[this->capacity]; // arreglo de punteros, uno por fila
    for (int i = 0; i < this->capacity; i++) {
        matrix[i] = new bool[this->capacity](); // cada fila, () la llena en false para que no quede basura
    }
}

// Implementacion del Destructor
AdjacencyMatrix::~AdjacencyMatrix(){

    for (int i = 0; i < capacity; i++) {
        delete[] matrix[i]; // primero cada fila
    }
    delete[] matrix; // despues el arreglo de punteros, si fuera primero perderiamos las filas
}

// Metodo para agrandar la matriz
void AdjacencyMatrix::resize(int newCapacity){

    bool** newMatrix = new bool*[newCapacity];
    for (int i = 0; i < newCapacity; i++) {
        newMatrix[i] = new bool[newCapacity](); // matriz nueva toda en false
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            newMatrix[i][j] = matrix[i][j]; // copiamos solo la parte que esta en uso (n x n)
        }
    }

    for (int i = 0; i < capacity; i++) {
        delete[] matrix[i]; // liberamos la vieja igual que en el destructor
    }
    delete[] matrix;

    matrix = newMatrix;
    capacity = newCapacity;
}

// Metodo que agrega un nodo (usuario) y retorna su indice
int AdjacencyMatrix::addNode(){

    if (n == capacity) {
        resize(capacity * 2); // duplicamos, igual que en DynamicList
    }
    return n++; // retorna el indice nuevo y despues aumenta n
}

// Metodo que registra una amistad entre i y j
void AdjacencyMatrix::addFriendship(int i, int j){

    if (i < 0 || i >= n || j < 0 || j >= n) {
        throw std::out_of_range("AdjacencyMatrix: index out of range"); // entre n y capacity no hay usuarios
    }
    if (i == j) return; // nadie puede ser amigo de si mismo

    matrix[i][j] = true;
    matrix[j][i] = true; // la amistad es mutua, la matriz es simetrica
}

// Metodo que dice si i y j son amigos
bool AdjacencyMatrix::areFriends(int i, int j) const {

    if (i < 0 || i >= n || j < 0 || j >= n) {
        throw std::out_of_range("AdjacencyMatrix: index out of range");
    }
    return matrix[i][j]; // acceso directo O(1)
}

// Metodo que retorna cuantos nodos hay
int AdjacencyMatrix::getN() const {

    return n;
}
