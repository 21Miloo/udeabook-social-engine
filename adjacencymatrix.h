#ifndef ADJACENCYMATRIX_H
#define ADJACENCYMATRIX_H

#include <stdexcept>

class AdjacencyMatrix{

private:

    bool** matrix; // arreglo de filas, cada fila es un arreglo de bool
    int n;         // cuantos usuarios (nodos) hay realmente
    int capacity;  // cuantos caben antes de tener que crecer

    void resize(int newCapacity); // solo la matriz decide cuando crecer

public:

    AdjacencyMatrix(int capacity = 10); // Constructor
    AdjacencyMatrix(const AdjacencyMatrix& other); // Constructor de copia
    AdjacencyMatrix& operator=(const AdjacencyMatrix& other); // Asignacion

    ~AdjacencyMatrix(); // Destructor

    void reserve(int newCapacity); // deja la matriz vacia con espacio para newCapacity nodos

    int addNode(); // agrega un usuario nuevo y retorna su indice

    void addFriendship(int i, int j); // marca a i y j como amigos

    bool areFriends(int i, int j) const; // dice si i y j son amigos. const porque solo consulta

    int getN() const; // cuantos nodos hay

    int calculateMemory() const; // cuantos bytes ocupa la matriz (para la medicion de recursos)

};

#endif // ADJACENCYMATRIX_H
