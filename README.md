# Ude@Book

Red social por consola desarrollada en C++ para el Desafio II de Informatica II (Universidad de Antioquia, 2026-2).

Los usuarios pueden iniciar sesion, ver a sus mejores amigos, enviar y aceptar solicitudes de amistad, ver sugerencias de amistad, visitar el perfil de un amigo y recorrer sus albumes de fotos en modo carrusel, y enviar y leer mensajes. Al terminar cada funcionalidad el programa muestra las iteraciones realizadas y la memoria que esta usando.

El proyecto usa programacion orientada a objetos y estructuras de datos propias con memoria dinamica (DynamicList y AdjacencyMatrix), sin contenedores de la STL, sin structs y sin herencia.

## Estructura

- Codigo fuente (.h y .cpp): en la raiz del repositorio.
- `data/`: archivos de texto con los usuarios, amistades, solicitudes, albumes, fotos y mensajes. Se cargan al iniciar y se guardan al salir.
- `tools/generator.cpp`: generador de los datos de prueba (1000 usuarios).
- `docs/`: diagrama de clases e informe.

## Compilacion y ejecucion

Requiere un compilador de C++17 y CMake 3.16 o superior.

```
cmake -S . -B build
cmake --build build
```

El programa debe ejecutarse desde la raiz del repositorio para que encuentre la carpeta `data/`. En Qt Creator se puede abrir el `CMakeLists.txt` y poner como directorio de trabajo la carpeta del proyecto.

## Autores

- Juan Esteban Lopez Castrillon
- Juan Camilo Lopez Castrillon
