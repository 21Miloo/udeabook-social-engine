// Generador de datos de prueba para UdeaBook
// Programa aparte (no hace parte del CMakeLists). Escribe los 6 archivos de data/ con el formato acordado.
//
// Compilar (desde la raiz del repo, en cmd):
//   set PATH=C:\Qt\Tools\mingw1310_64\bin;%PATH%
//   g++ -std=c++17 -static tools/generator.cpp -o generator.exe
// Ejecutar:
//   generator.exe data/

#include <iostream>
#include <fstream>
#include <string>
#include <random>

using namespace std;

const int TOTAL_USERS = 1000;
const int GROUP_SIZE = 60;          // los usuarios se agrupan como en cursos o barrios
const int CROSS_FRIENDS = 3;        // amistades por usuario con gente de otros grupos
const int RANDOM_REQUESTS = 300;    // solicitudes pendientes entre usuarios cualquiera
const int RANDOM_MESSAGES = 3000;   // mensajes entre amigos cualquiera
const int DEMO_NOT_FRIENDS = 8;     // del grupo del usuario del video, cuantos NO son sus amigos (salen como sugerencias)
const int DEMO_REQUESTS = 3;        // de esos, cuantos ya le enviaron solicitud

// relacion entre dos usuarios en la matriz
const char NONE = 0;
const char FRIENDS = 1;
const char REQUEST = 2; // rel[i][j] == REQUEST significa que i le envio solicitud a j

mt19937 generator(2026); // semilla fija: siempre se generan los mismos datos

string* names = nullptr;     // names[i] es el userName del usuario i
string* passwords = nullptr; // passwords[i] es la clave del usuario i

// Funcion que retorna un entero al azar entre min y max (incluidos)
int randomInt(int min, int max){
    uniform_int_distribution<int> range(min, max);
    return range(generator);
}

// Funcion que retorna true con probabilidad p (entre 0 y 1)
bool chance(double p){
    uniform_real_distribution<double> range(0.0, 1.0);
    return range(generator) < p;
}

// Funcion que completa un numero con ceros a la izquierda (7, 4 -> "0007")
string pad(int number, int digits){
    string text = to_string(number);
    while ((int)text.length() < digits) text = "0" + text;
    return text;
}

// Funcion que retorna el nombre de usuario del usuario i
string userName(int i){
    return names[i];
}

// Funcion que crea los nombres combinando nombres y apellidos, sin repetir. El usuario 0 es el del video
void createNames(){

    const int FIRST = 40;
    const int LAST = 40;
    string firstNames[FIRST] = {
        "camilo", "esteban", "laura", "andres", "valentina", "santiago", "daniela", "juan", "mariana", "sebastian",
        "sara", "mateo", "isabella", "nicolas", "manuela", "felipe", "sofia", "david", "natalia", "alejandro",
        "paula", "julian", "carolina", "miguel", "luisa", "diego", "catalina", "simon", "juliana", "tomas",
        "gabriela", "samuel", "antonia", "jeronimo", "maria", "pablo", "salome", "emilio", "veronica", "martin"
    };
    string lastNames[LAST] = {
        "lopez", "gomez", "restrepo", "garcia", "rodriguez", "martinez", "hernandez", "ramirez", "torres", "vargas",
        "jaramillo", "castro", "ospina", "zapata", "mejia", "cardona", "henao", "giraldo", "arango", "rios",
        "munoz", "sanchez", "diaz", "moreno", "rojas", "alvarez", "gutierrez", "quintero", "salazar", "velez",
        "correa", "posada", "uribe", "herrera", "medina", "agudelo", "montoya", "londono", "escobar", "suarez"
    };

    // cada combinacion es un numero: nombre * LAST + apellido. La 0 es camilo_lopez, la apartamos para el video
    int totalCombinations = FIRST * LAST - 1;
    int* combinations = new int[totalCombinations];
    for (int k = 0; k < totalCombinations; k++) combinations[k] = k + 1; // de 1 en adelante, sin la 0

    // barajamos (Fisher-Yates): cada posicion se cambia con una al azar de las que faltan
    for (int k = totalCombinations - 1; k > 0; k--) {
        int r = randomInt(0, k);
        int temp = combinations[k];
        combinations[k] = combinations[r];
        combinations[r] = temp;
    }

    names = new string[TOTAL_USERS];
    passwords = new string[TOTAL_USERS];

    names[0] = firstNames[0] + "_" + lastNames[0]; // camilo_lopez
    passwords[0] = firstNames[0] + "123";
    for (int i = 1; i < TOTAL_USERS; i++) {
        int first = combinations[i - 1] / LAST;
        int last = combinations[i - 1] % LAST;
        names[i] = firstNames[first] + "_" + lastNames[last];
        passwords[i] = firstNames[first] + "123"; // ej: laura_gomez -> laura123
    }

    delete[] combinations;
}

// Funcion que arma una fecha aaaa-mm-dd
string dateText(int y, int m, int d){
    return to_string(y) + "-" + pad(m, 2) + "-" + pad(d, 2);
}

// Funcion que arma una fecha aaaa-mm-dd hh:mm
string dateTimeText(int y, int m, int d, int h, int min){
    return dateText(y, m, d) + " " + pad(h, 2) + ":" + pad(min, 2);
}

// Funcion que dice cuantos dias tiene un mes
int daysInMonth(int m, int y){
    int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
    if (m == 2 && leap) return 29;
    return days[m - 1];
}

// Funcion que avanza una fecha cierta cantidad de minutos (para que los mensajes queden ordenados)
void addMinutes(int& y, int& m, int& d, int& h, int& min, int minutes){
    min += minutes;
    while (min >= 60) { min -= 60; h++; }
    while (h >= 24) { h -= 24; d++; }
    while (d > daysInMonth(m, y)) {
        d -= daysInMonth(m, y);
        m++;
        if (m > 12) { m = 1; y++; }
    }
}

// Funcion que escribe users.txt
void writeUsers(const string& path){

    const int CITIES = 8;
    string cities[CITIES]    = {"Medellin", "Bogota", "Cali", "Barranquilla", "Lima", "Quito", "Santiago", "Ciudad de Mexico"};
    string countries[CITIES] = {"Colombia", "Colombia", "Colombia", "Colombia", "Peru", "Ecuador", "Chile", "Mexico"};

    // users.txt debe ir ordenado por nombre: ordenamos las posiciones (insercion), no los usuarios
    int* order = new int[TOTAL_USERS];
    for (int i = 0; i < TOTAL_USERS; i++) order[i] = i;
    for (int i = 1; i < TOTAL_USERS; i++) {
        int current = order[i];
        int j = i - 1;
        while (j >= 0 && names[order[j]] > names[current]) { // corremos a la derecha los que van despues
            order[j + 1] = order[j];
            j--;
        }
        order[j + 1] = current;
    }

    ofstream file(path + "users.txt");
    file << TOTAL_USERS << '\n';

    for (int k = 0; k < TOTAL_USERS; k++) {
        int i = order[k]; // el usuario que va en la posicion k del archivo
        int c = (i < 4) ? 0 : randomInt(0, CITIES - 1); // los primeros quedan en Medellin para el video
        if (c > 3 && chance(0.5)) c = randomInt(0, 3);  // la mayoria en Colombia

        file << userName(i) << "|"
             << passwords[i] << "|"
             << userName(i) << "@udea.edu.co" << "|"
             << dateText(randomInt(1995, 2007), randomInt(1, 12), randomInt(1, 28)) << "|"
             << cities[c] << "|" << countries[c] << "|"
             << dateTimeText(randomInt(2023, 2025), randomInt(1, 12), randomInt(1, 28), randomInt(0, 23), randomInt(0, 59))
             << '\n';
    }
    delete[] order;
}

// Funcion que crea las amistades y solicitudes en la matriz rel
void createRelations(char** rel){

    // cada usuario tiene una "sociabilidad": los muy sociables tienen muchos amigos en su grupo
    double* social = new double[TOTAL_USERS];
    for (int i = 0; i < TOTAL_USERS; i++) {
        social[i] = 0.15 + 0.85 * (randomInt(0, 1000) / 1000.0); // entre 0.15 y 1
    }

    // amistades dentro del grupo: la probabilidad es social[i] * social[j]
    for (int i = 0; i < TOTAL_USERS; i++) {
        int groupEnd = (i / GROUP_SIZE + 1) * GROUP_SIZE;
        for (int j = i + 1; j < groupEnd && j < TOTAL_USERS; j++) {
            if (chance(social[i] * social[j])) {
                rel[i][j] = FRIENDS;
                rel[j][i] = FRIENDS;
            }
        }
    }

    // el usuario del video (camilo_lopez, posicion 0): amigo de casi todo su grupo, menos los ultimos DEMO_NOT_FRIENDS
    int lastFriend = GROUP_SIZE - DEMO_NOT_FRIENDS;
    for (int j = 1; j < GROUP_SIZE; j++) {
        char value = (j < lastFriend) ? FRIENDS : NONE;
        rel[0][j] = value;
        rel[j][0] = value;
    }

    // amistades con otros grupos (son las que conectan la red)
    for (int i = 0; i < TOTAL_USERS; i++) {
        for (int k = 0; k < CROSS_FRIENDS; k++) {
            int j = randomInt(0, TOTAL_USERS - 1);
            if (j == i || j / GROUP_SIZE == i / GROUP_SIZE) continue; // mismo usuario o mismo grupo
            if (i < GROUP_SIZE && i >= lastFriend) continue; // dejamos a los no-amigos del video sin cambios raros
            rel[i][j] = FRIENDS;
            rel[j][i] = FRIENDS;
        }
    }

    // solicitudes pendientes hacia el usuario del video
    for (int k = 0; k < DEMO_REQUESTS; k++) {
        rel[lastFriend + k][0] = REQUEST;
    }

    // solicitudes pendientes al azar entre usuarios que no son amigos
    int created = 0;
    while (created < RANDOM_REQUESTS) {
        int a = randomInt(1, TOTAL_USERS - 1);
        int b = randomInt(1, TOTAL_USERS - 1);
        if (a == b || rel[a][b] != NONE || rel[b][a] != NONE) continue; // ya son amigos o ya hay solicitud en algun sentido
        rel[a][b] = REQUEST;
        created++;
    }

    delete[] social;
}

// Funcion que escribe friendships.txt y requests.txt a partir de la matriz
void writeRelations(const string& path, char** rel){

    ofstream friendships(path + "friendships.txt");
    ofstream requests(path + "requests.txt");

    int totalFriendships = 0;
    for (int i = 0; i < TOTAL_USERS; i++) {
        for (int j = 0; j < TOTAL_USERS; j++) {
            if (rel[i][j] == FRIENDS && i < j) { // cada amistad una sola vez
                friendships << userName(i) << "|" << userName(j) << '\n';
                totalFriendships++;
            }
            if (rel[i][j] == REQUEST) {
                requests << userName(i) << "|" << userName(j) << '\n'; // sender|receiver
            }
        }
    }
    cout << "Amistades: " << totalFriendships << endl;
}

// Funcion que escribe messages.txt (solo entre amigos y ordenado por fecha)
void writeMessages(const string& path, char** rel){

    const int TEXTS = 12;
    string texts[TEXTS] = {
        "Hola, como vas?", "Bien y tu?", "Ya hiciste el taller de Informatica II?",
        "Nos vemos manana en la U", "Que mas pues", "Me pasas los apuntes de calculo?",
        "Jajaja", "Listo, hablamos luego", "Viste el partido?", "Feliz cumpleanos!",
        "A que hora es el parcial?", "Gracias | te debo una"
    };

    ofstream file(path + "messages.txt");

    int y = 2025, m = 1, d = 1, h = 8, min = 0; // fecha del primer mensaje

    int written = 0;
    while (written < RANDOM_MESSAGES) {
        int a = randomInt(1, TOTAL_USERS - 1);
        int b = randomInt(1, TOTAL_USERS - 1);
        if (rel[a][b] != FRIENDS) continue; // solo entre amigos

        addMinutes(y, m, d, h, min, randomInt(1, 200));
        file << userName(a) << "|" << userName(b) << "|" << dateTimeText(y, m, d, h, min) << "|"
             << (chance(0.8) ? 1 : 0) << "|" << texts[randomInt(0, TEXTS - 1)] << '\n';
        written++;
    }

    // conversaciones del usuario del video con sus primeros 5 amigos: al final para que sean las mas recientes
    // entre mensaje y mensaje pasan de 2.4 a 4 dias, asi quedan repartidas en mas de 6 semanas
    for (int f = 1; f <= 5; f++) {
        for (int k = 0; k < 4; k++) {
            addMinutes(y, m, d, h, min, randomInt(3500, 6000));
            bool fromDemo = (k % 2 == 1);
            int sender = fromDemo ? 0 : f;
            int receiver = fromDemo ? f : 0;
            int read = (k == 2) ? 0 : 1; // en cada conversacion queda un mensaje sin leer para camilo_lopez
            file << userName(sender) << "|" << userName(receiver) << "|" << dateTimeText(y, m, d, h, min) << "|"
                 << read << "|" << texts[randomInt(0, TEXTS - 1)] << '\n';
        }
    }
}

// Funcion que escribe albums.txt y photos.txt
void writeAlbums(const string& path){

    const int NAMES = 6;
    string albumNames[NAMES] = {"Viajes", "Familia", "Universidad", "Amigos", "Mascotas", "Eventos"};
    string privacy[3] = {"PUBLIC", "PRIVATE", "FRIENDS"};

    ofstream albums(path + "albums.txt");
    ofstream photos(path + "photos.txt");

    int totalPhotos = 0;
    for (int i = 0; i < TOTAL_USERS; i++) {

        int count = (i == 0) ? 3 : randomInt(1, 3); // el usuario del video tiene los 3 tipos de privacidad
        int first = randomInt(0, NAMES - 1);

        for (int a = 0; a < count; a++) {
            string album = albumNames[(first + a) % NAMES]; // nombres distintos dentro del mismo usuario
            string albumPrivacy = (i == 0) ? privacy[a] : privacy[randomInt(0, 2)];
            albums << userName(i) << "|" << album << "|" << albumPrivacy << '\n';

            int photoCount = randomInt(2, 6);
            for (int p = 0; p < photoCount; p++) {
                photos << userName(i) << "|" << album << "|" << "foto" << pad(p + 1, 2) << "|"
                       << dateTimeText(randomInt(2023, 2025), randomInt(1, 12), randomInt(1, 28), randomInt(0, 23), randomInt(0, 59)) << "|"
                       << randomInt(100000, 3000000) << "|"   // tamano en bytes
                       << randomInt(0, 200) << '\n';          // likes
                totalPhotos++;
            }
        }
    }
    cout << "Fotos: " << totalPhotos << endl;
}

int main(int argc, char* argv[])
{
    string path = (argc > 1) ? argv[1] : "data/";
    if (path[path.length() - 1] != '/') path += "/";

    // matriz de relaciones: igual idea que AdjacencyMatrix, pero con 3 estados
    char** rel = new char*[TOTAL_USERS];
    for (int i = 0; i < TOTAL_USERS; i++) {
        rel[i] = new char[TOTAL_USERS](); // todo en NONE
    }

    createNames();
    writeUsers(path);
    createRelations(rel);
    writeRelations(path, rel);
    writeMessages(path, rel);
    writeAlbums(path);

    for (int i = 0; i < TOTAL_USERS; i++) delete[] rel[i];
    delete[] rel;

    int lastFriend = GROUP_SIZE - DEMO_NOT_FRIENDS;
    cout << "Usuario del video: " << names[0] << " / " << passwords[0] << endl;
    cout << "Le enviaron solicitud:";
    for (int k = 0; k < DEMO_REQUESTS; k++) cout << " " << names[lastFriend + k];
    cout << endl;

    delete[] names;
    delete[] passwords;

    cout << "Datos generados en " << path << endl;
    return 0;
}
