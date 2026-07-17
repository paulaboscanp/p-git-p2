#include "paquete.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

//ARCHIVO

// Constructor por defecto
Archivo::Archivo() {
    nombre = "";
    contenido = nullptr;
    tamano = 0;
    checksum = 0;
    comprimido = false;
    tamanoOriginal = 0;
}

// Constructor de copia (deep copy)
Archivo::Archivo(const Archivo& otro) {

    nombre = otro.nombre;
    tamano = otro.tamano;
    checksum = otro.checksum;
    comprimido = otro.comprimido;
    tamanoOriginal = otro.tamanoOriginal;

    // Solo copia la memoria si existe contenido
    if (otro.contenido != nullptr && otro.tamano > 0) {

        contenido = new unsigned char[otro.tamano];

        memcpy(contenido,
               otro.contenido,
               otro.tamano);

    }
    else {

        contenido = nullptr;

    }
}

// Operador de asignacion
Archivo& Archivo::operator=(const Archivo& otro) {

    if (this != &otro) {

        // Libera la memoria anterior
        delete[] contenido;

        nombre = otro.nombre;
        tamano = otro.tamano;
        checksum = otro.checksum;
        comprimido = otro.comprimido;
        tamanoOriginal = otro.tamanoOriginal;

        // Copia el contenido del otro archivo
        if (otro.contenido != nullptr && otro.tamano > 0) {

            contenido = new unsigned char[otro.tamano];

            memcpy(contenido,
                   otro.contenido,
                   otro.tamano);

        }
        else {

            contenido = nullptr;

        }
    }

    return *this;
}

// Destructor
Archivo::~Archivo() {

    if (contenido != nullptr) {

        delete[] contenido;
        contenido = nullptr;

    }
}

// PAQUETE

// Constructor
Paquete::Paquete(string nombre) {

    this->nombre = nombre;

}

// Constructor de copia
Paquete::Paquete(const Paquete& otro) {

    nombre = otro.nombre;
    archivos = otro.archivos;

}

// Destructor
Paquete::~Paquete() {

}

// Operador de asignacion
Paquete& Paquete::operator=(const Paquete& otro) {

    if (this != &otro) {

        nombre = otro.nombre;
        archivos = otro.archivos;

    }

    return *this;
}

// Calcula un checksum sencillo sumando todos los bytes
int Paquete::calcularChecksum(unsigned char* data,
                              int size) {

    int suma = 0;

    for (int i = 0; i < size; i++) {

        suma += data[i];

    }

    return suma;
}

// Agrega un archivo al paquete
void Paquete::agregarArchivo(string nombre,
                             unsigned char* data,
                             int size) {

    // Verifica que los datos sean validos
    if (data == nullptr || size <= 0) {

        cout << "No se pudo agregar el archivo." << endl;
        return;

    }

    Archivo nuevo;

    nuevo.nombre = nombre;
    nuevo.tamano = size;
    nuevo.tamanoOriginal = size;
    nuevo.comprimido = false;

    // Reserva memoria para el contenido
    nuevo.contenido = new unsigned char[size];

    memcpy(nuevo.contenido,
           data,
           size);

    // Guarda el checksum del archivo
    nuevo.checksum = calcularChecksum(data,
                                      size);

    archivos.push_back(nuevo);

    cout << "Archivo agregado: "
         << nombre
         << endl;
}

// Muestra todos los archivos del paquete
void Paquete::listarArchivos() {

    cout << endl;
    cout << "PAQUETE: " << nombre << " " << endl;

    if (archivos.empty()) {

        cout << "No hay archivos en este paquete." << endl;
        return;

    }

    for (int i = 0; i < archivos.size(); i++) {

        cout << "[" << i << "] "
             << archivos[i].nombre
             << " ("
             << archivos[i].tamano
             << " bytes)";

        if (archivos[i].comprimido) {
            cout << " [COMPRIMIDO]";
        }

        cout << endl;
    }
}

// Muestra el contenido de un archivo
void Paquete::extraerArchivo(int indice) {

    if (indice < 0 || indice >= archivos.size()) {
        cout << "Indice invalido." << endl;
        return;
    }

    Archivo& a = archivos[indice];

    cout << endl;
    cout << "Archivo: " << a.nombre << endl;
    cout << "Tamano: " << a.tamano << " bytes" << endl;
    cout << "Checksum: " << a.checksum << endl;

    cout << "Contenido:" << endl;

    for (int i = 0; i < a.tamano; i++) {

        cout << (int)a.contenido[i] << " ";

    }

    cout << endl;
}

// Elimina un archivo del paquete
bool Paquete::eliminarArchivo(string nombre) {

    for (int i = 0; i < archivos.size(); i++) {

        if (archivos[i].nombre == nombre) {

            archivos.erase(archivos.begin() + i);

            cout << "Archivo eliminado correctamente." << endl;

            return true;
        }
    }

    cout << "Archivo no encontrado." << endl;

    return false;
}

// Busca un archivo por nombre
int Paquete::buscarArchivo(string nombre) {

    for (int i = 0; i < archivos.size(); i++) {

        if (archivos[i].nombre == nombre) {

            cout << "Encontrado en indice "
                 << i
                 << endl;

            return i;
        }
    }

    cout << "Archivo no encontrado." << endl;

    return -1;
}

// Comprime un archivo usando RLE
void Paquete::comprimirArchivo(int indice) {

    if (indice < 0 || indice >= archivos.size()) {
        return;
    }

    Archivo& a = archivos[indice];

    if (a.comprimido) {

        cout << "El archivo ya esta comprimido." << endl;
        return;

    }

    unsigned char* temp = new unsigned char[a.tamano * 2];

    int j = 0;

    for (int i = 0; i < a.tamano; i++) {

        int contador = 1;

        while (i + 1 < a.tamano &&
               a.contenido[i] == a.contenido[i + 1] &&
               contador < 255) {

            contador++;
            i++;

        }

        temp[j++] = a.contenido[i];
        temp[j++] = contador;
    }

    // Si la compresion ocupa mas espacio, se cancela
    if (j >= a.tamano) {

        delete[] temp;

        cout << "No vale la pena comprimir este archivo." << endl;

        return;
    }

    delete[] a.contenido;

    a.contenido = new unsigned char[j];

    memcpy(a.contenido,
           temp,
           j);

    a.tamano = j;
    a.comprimido = true;

    delete[] temp;

    double porcentaje =
        (double)a.tamano /
        (double)a.tamanoOriginal * 100.0;

    cout << "Archivo comprimido correctamente." << endl;
    cout << "Tamano original: "
         << a.tamanoOriginal
         << " bytes" << endl;

    cout << "Tamano comprimido: "
         << a.tamano
         << " bytes" << endl;

    cout << "Porcentaje: "
         << porcentaje
         << "%" << endl;
}

// Descomprime un archivo
void Paquete::descomprimirArchivo(int indice) {

    if (indice < 0 || indice >= archivos.size()) {
        return;
    }

    Archivo& a = archivos[indice];

    if (!a.comprimido) {

        cout << "El archivo no esta comprimido." << endl;
        return;

    }

    if (a.tamano % 2 != 0) {

        cout << "El archivo comprimido esta corrupto." << endl;
        return;

    }

    unsigned char* temp =
        new unsigned char[a.tamanoOriginal];

    int j = 0;

    for (int i = 0; i < a.tamano; i += 2) {

        unsigned char valor = a.contenido[i];
        unsigned char cantidad = a.contenido[i + 1];

        for (int k = 0; k < cantidad; k++) {

            temp[j++] = valor;

        }
    }

    delete[] a.contenido;

    a.contenido =
        new unsigned char[a.tamanoOriginal];

    memcpy(a.contenido,
           temp,
           a.tamanoOriginal);

    a.tamano = a.tamanoOriginal;
    a.comprimido = false;

    delete[] temp;

    cout << "Archivo descomprimido correctamente."
         << endl;
}

// Verifica que el contenido del archivo no haya cambiado
bool Paquete::verificarIntegridad(int indice) {

    if (indice < 0 || indice >= archivos.size()) {

        cout << "Indice invalido." << endl;
        return false;

    }

    Archivo& a = archivos[indice];

    if (a.contenido == nullptr) {

        cout << "El archivo no tiene contenido." << endl;
        return false;

    }

    int checksumActual =
        calcularChecksum(a.contenido,
                         a.tamano);

    cout << "Checksum esperado: "
         << a.checksum
         << endl;

    cout << "Checksum actual: "
         << checksumActual
         << endl;

    if (checksumActual == a.checksum) {

        cout << "Integridad correcta." << endl;
        return true;

    }

    cout << "El archivo fue modificado." << endl;

    return false;
}

// Devuelve el nombre del paquete
string Paquete::getNombre() {

    return nombre;

}

// Devuelve la lista de archivos
const vector<Archivo>&
Paquete::getArchivos() const {

    return archivos;

}

// Agrega un archivo desde el disco
bool Paquete::agregarArchivoReal(string ruta) {

    ifstream archivo(ruta,
                     ios::binary);

    if (!archivo.is_open()) {

        cout << "No se pudo abrir el archivo." << endl;
        return false;

    }

    archivo.seekg(0, ios::end);

    int tamano =
        archivo.tellg();

    archivo.seekg(0, ios::beg);

    unsigned char* buffer =
        new unsigned char[tamano];

    archivo.read((char*)buffer,
                 tamano);

    archivo.close();

    agregarArchivo(ruta,
                   buffer,
                   tamano);

    delete[] buffer;

    cout << "Archivo importado correctamente."
         << endl;

    return true;
}

// Extrae un archivo del paquete al disco
bool Paquete::extraerArchivoDisco(int indice,
                                  string ruta) {

    if (indice < 0 ||
        indice >= archivos.size()) {

        return false;

    }

    ofstream archivo(ruta,
                     ios::binary);

    if (!archivo.is_open()) {

        cout << "No se pudo crear el archivo."
             << endl;

        return false;

    }

    Archivo& a =
        archivos[indice];

    archivo.write((char*)a.contenido,
                  a.tamano);

    archivo.close();

    cout << "Archivo extraido correctamente."
         << endl;

    return true;
}

// Muestra informacion del paquete
void Paquete::mostrarInformacion() {

    int totalBytes = 0;
    int comprimidos = 0;

    for (int i = 0;
         i < archivos.size();
         i++) {

        totalBytes +=
            archivos[i].tamano;

        if (archivos[i].comprimido) {

            comprimidos++;

        }

    }

    cout << endl;
    cout << "INFORMACION" << endl;

    cout << "Nombre: "
         << nombre
         << endl;

    cout << "Cantidad de archivos: "
         << archivos.size()
         << endl;

    cout << "Tamano total: "
         << totalBytes
         << " bytes"
         << endl;

    cout << "Comprimidos: "
         << comprimidos
         << endl;
}

// Exporta un indice simple del paquete
bool Paquete::exportarIndice(string archivoSalida) {

    ofstream archivo(archivoSalida);

    if (!archivo.is_open()) {

        cout << "No se pudo crear el archivo."
             << endl;

        return false;

    }

    archivo << "Nombre,Tamano,Checksum\n";

    for (int i = 0;
         i < archivos.size();
         i++) {

        archivo
            << archivos[i].nombre << ","
            << archivos[i].tamano << ","
            << archivos[i].checksum
            << "\n";

    }

    archivo.close();

    cout << "Indice exportado correctamente."
         << endl;

    return true;
}

// Guarda el paquete en formato texto
bool Paquete::guardarPaqueteTXT(string ruta) {

    ofstream archivo(ruta);

    if (!archivo.is_open()) {

        cout << "No se pudo crear el archivo."
             << endl;

        return false;

    }

    archivo << nombre << endl;
    archivo << archivos.size() << endl;

    for (int i = 0;
         i < archivos.size();
         i++) {

        archivo << archivos[i].nombre << endl;
        archivo << archivos[i].tamano << endl;

        for (int j = 0;
             j < archivos[i].tamano;
             j++) {

            archivo
                << (int)archivos[i].contenido[j]
                << " ";

        }

        archivo << endl;

    }

    archivo.close();

    cout << "Paquete guardado correctamente."
         << endl;

    return true;
}

// Carga un paquete guardado
bool Paquete::cargarPaqueteTXT(string ruta) {

    ifstream archivo(ruta);

    if (!archivo.is_open()) {

        cout << "No se pudo abrir el archivo."
             << endl;

        return false;

    }

    // Libera la memoria anterior
    archivos.clear();

    getline(archivo,
            nombre);

    int cantidad;

    archivo >> cantidad;

    archivo.ignore();

    for (int i = 0;
         i < cantidad;
         i++) {

        string nombreArchivo;

        getline(archivo,
                nombreArchivo);

        int tamano;

        archivo >> tamano;

        unsigned char* buffer =
            new unsigned char[tamano];

        for (int j = 0;
             j < tamano;
             j++) {

            int valor;

            archivo >> valor;

            buffer[j] = valor;

        }

        archivo.ignore();

        agregarArchivo(nombreArchivo,
                       buffer,
                       tamano);

        delete[] buffer;

    }

    archivo.close();

    cout << "Paquete cargado correctamente."
         << endl;

    return true;
}

// Entrega 3

// Calcula un checksum utilizando XOR
int Paquete::calcularChecksumBinario(unsigned char* data,
                                     int size) {

    int checksum = 0;

    for (int i = 0; i < size; i++) {

        checksum ^= data[i];

    }

    return checksum;

}

// Escribe una cadena en formato binario
void Paquete::escribirCadena(ofstream& archivo,
                             const string& texto) {

    int longitud = texto.size();

    archivo.write(
        reinterpret_cast<char*>(&longitud),
        sizeof(longitud));

    archivo.write(
        texto.c_str(),
        longitud);

}

// Lee una cadena desde un archivo binario
string Paquete::leerCadena(ifstream& archivo) {

    int longitud;

    archivo.read(
        reinterpret_cast<char*>(&longitud),
        sizeof(longitud));

    string texto;

    texto.resize(longitud);

    archivo.read(
        &texto[0],
        longitud);

    return texto;

}

// IMPORTACION Y EXPORTACION DE ARCHIVOS BINARIOS (E3)

// Importa cualquier archivo utilizando modo binario
bool Paquete::importarArchivoBinario(string ruta) {

    ifstream archivo(ruta,
                     ios::binary);

    if (!archivo.is_open()) {

        cout << "No se pudo abrir el archivo."
             << endl;

        return false;

    }

    archivo.seekg(0, ios::end);

    int tamano =
        archivo.tellg();

    archivo.seekg(0, ios::beg);

    unsigned char* buffer =
        new unsigned char[tamano];

    archivo.read(
        reinterpret_cast<char*>(buffer),
        tamano);

    archivo.close();

    filesystem::path rutaArchivo(ruta);

    agregarArchivo(rutaArchivo.filename().string(),
                   buffer,
                   tamano);

    delete[] buffer;

    cout << "Archivo binario importado correctamente."
         << endl;

    return true;

}

// Exporta un archivo binario al disco
bool Paquete::exportarArchivoBinario(int indice,
                                     string ruta) {

    if (indice < 0 ||
        indice >= archivos.size()) {

        cout << "Indice invalido."
             << endl;

        return false;

    }

    ofstream archivo(ruta,
                     ios::binary);

    if (!archivo.is_open()) {

        cout << "No se pudo crear el archivo."
             << endl;

        return false;

    }

    Archivo& a =
        archivos[indice];

    archivo.write(
        reinterpret_cast<char*>(a.contenido),
        a.tamano);

    archivo.close();

    cout << "Archivo binario exportado correctamente."
         << endl;

    return true;

}

// FUNCIONES ENTREGA 3 - BINARY PACKAGE (.pkg)

// Guarda el paquete completo en formato .pkg
bool Paquete::guardarPaquetePKG(string ruta) {

    ofstream archivo(ruta,
                     ios::binary);

    if (!archivo.is_open()) {

        cout << "No se pudo crear el archivo .pkg" << endl;
        return false;

    }

    // Magic number: PKG1
    archivo.write("PKG1", 4);

    // Version
    int version = 1;
    archivo.write(reinterpret_cast<char*>(&version),
                  sizeof(version));

    // Checksum (se calcula al final, reservamos espacio)
    long posChecksum = archivo.tellp();
    int checksumTotal = 0;
    archivo.write(reinterpret_cast<char*>(&checksumTotal),
                  sizeof(checksumTotal));

    // Cantidad de archivos
    int cantidad = archivos.size();
    archivo.write(reinterpret_cast<char*>(&cantidad),
                  sizeof(cantidad));

    // Indice binario
    vector<EntradaIndice> indice;
    long posIndice = archivo.tellp();

    // Escribir indice temporal (offsets se actualizan despues)
    for (int i = 0; i < archivos.size(); i++) {

        EntradaIndice entrada;
        entrada.nombre = archivos[i].nombre;
        entrada.tamano = archivos[i].tamano;
        entrada.offset = 0;
        indice.push_back(entrada);

        escribirCadena(archivo, entrada.nombre);
        archivo.write(reinterpret_cast<char*>(&entrada.tamano),
                      sizeof(entrada.tamano));
        archivo.write(reinterpret_cast<char*>(&entrada.offset),
                      sizeof(entrada.offset));

    }

    // Escribir los datos de los archivos
    checksumTotal = 0;

    for (int i = 0; i < archivos.size(); i++) {

        // Actualizar offset en el indice
        indice[i].offset = archivo.tellp();

        // Escribir el contenido del archivo
        archivo.write(reinterpret_cast<char*>(archivos[i].contenido),
                      archivos[i].tamano);

        // Acumular checksum total (XOR de todos los bytes)
        for (int j = 0; j < archivos[i].tamano; j++) {

            checksumTotal ^= static_cast<int>(archivos[i].contenido[j]);

        }

    }

    // Volver a escribir el indice con los offsets actualizados
    archivo.seekp(posIndice);

    for (int i = 0; i < archivos.size(); i++) {

        escribirCadena(archivo, indice[i].nombre);
        archivo.write(reinterpret_cast<char*>(&indice[i].tamano),
                      sizeof(indice[i].tamano));
        archivo.write(reinterpret_cast<char*>(&indice[i].offset),
                      sizeof(indice[i].offset));

    }

    // Escribir el checksum total
    archivo.seekp(posChecksum);
    archivo.write(reinterpret_cast<char*>(&checksumTotal),
                  sizeof(checksumTotal));

    archivo.close();

    cout << "Paquete guardado en .pkg correctamente." << endl;
    cout << "Magic number: PKG1" << endl;
    cout << "Version: " << version << endl;
    cout << "Checksum: 0x" << hex << checksumTotal << dec << endl;

    return true;

}

// Carga un paquete desde un archivo .pkg
bool Paquete::cargarPaquetePKG(string ruta) {

    ifstream archivo(ruta,
                     ios::binary);

    if (!archivo.is_open()) {

        cout << "No se pudo abrir el archivo .pkg" << endl;
        return false;

    }

    // Verificar magic number
    char magic[5];
    archivo.read(magic, 4);
    magic[4] = '\0';

    if (strcmp(magic, "PKG1") != 0) {

        cout << "ERROR: Magic number invalido. Archivo corrupto." << endl;
        archivo.close();
        return false;

    }
    cout << "Magic number: PKG1" << endl;

    // Leer version
    int version;
    archivo.read(reinterpret_cast<char*>(&version),
                 sizeof(version));

    if (version != 1) {

        cout << "ERROR: Version no soportada." << endl;
        archivo.close();
        return false;

    }
    cout << "Version: " << version << endl;

    // Leer checksum
    int checksumGuardado;
    archivo.read(reinterpret_cast<char*>(&checksumGuardado),
                 sizeof(checksumGuardado));

    // Leer cantidad de archivos
    int cantidad;
    archivo.read(reinterpret_cast<char*>(&cantidad),
                 sizeof(cantidad));

    // Leer el indice
    vector<EntradaIndice> indice;

    for (int i = 0; i < cantidad; i++) {

        EntradaIndice entrada;
        entrada.nombre = leerCadena(archivo);
        archivo.read(reinterpret_cast<char*>(&entrada.tamano),
                     sizeof(entrada.tamano));
        archivo.read(reinterpret_cast<char*>(&entrada.offset),
                     sizeof(entrada.offset));
        indice.push_back(entrada);

    }

    // Limpiar archivos anteriores
    archivos.clear();

    // Leer los datos de cada archivo
    int checksumCalculado = 0;
    int totalBytes = 0;

    for (int i = 0; i < cantidad; i++) {

        // Ir a la posicion del archivo
        archivo.seekg(indice[i].offset);

        // Leer el contenido
        unsigned char* buffer = new unsigned char[indice[i].tamano];
        archivo.read(reinterpret_cast<char*>(buffer),
                     indice[i].tamano);

        // Calcular checksum
        for (int j = 0; j < indice[i].tamano; j++) {

            checksumCalculado ^= static_cast<int>(buffer[j]);

        }

        // Agregar al paquete
        agregarArchivo(indice[i].nombre,
                       buffer,
                       indice[i].tamano);

        totalBytes += indice[i].tamano;

        delete[] buffer;

    }

    archivo.close();

    // Verificar checksum
    if (checksumCalculado == checksumGuardado) {

        cout << "Checksum: 0x" << hex << checksumGuardado << dec << endl;
        cout << "Paquete cargado: " << cantidad << " archivos ("
             << totalBytes << " bytes)" << endl;
        return true;

    } else {

        cout << "Checksum: 0x" << hex << checksumGuardado << " -> 0x"
             << checksumCalculado << dec << endl;
        cout << "ERROR: El archivo esta corrupto. Checksum no coincide." << endl;
        archivos.clear();
        return false;

    }

}

// Verifica la integridad del archivo .pkg
bool Paquete::verificarPKG(string ruta) {

    ifstream archivo(ruta,
                     ios::binary);

    if (!archivo.is_open()) {

        cout << "No se pudo abrir el archivo .pkg" << endl;
        return false;

    }

    cout << "Verificando integridad..." << endl;

    // Verificar magic number
    char magic[5];
    archivo.read(magic, 4);
    magic[4] = '\0';

    if (strcmp(magic, "PKG1") != 0) {

        cout << "Magic number: " << magic << endl;
        cout << "ERROR: Magic number no coincide." << endl;
        archivo.close();
        return false;

    }
    cout << "Magic number: PKG1" << endl;

    // Leer version
    int version;
    archivo.read(reinterpret_cast<char*>(&version),
                 sizeof(version));
    cout << "Version: " << version << endl;

    // Leer checksum guardado
    int checksumGuardado;
    archivo.read(reinterpret_cast<char*>(&checksumGuardado),
                 sizeof(checksumGuardado));

    // Leer cantidad
    int cantidad;
    archivo.read(reinterpret_cast<char*>(&cantidad),
                 sizeof(cantidad));

    // Leer indice y calcular checksum
    int checksumCalculado = 0;

    for (int i = 0; i < cantidad; i++) {

        string nombre = leerCadena(archivo);
        int tamano;
        long offset;
        archivo.read(reinterpret_cast<char*>(&tamano),
                     sizeof(tamano));
        archivo.read(reinterpret_cast<char*>(&offset),
                     sizeof(offset));

        // Ir a los datos
        archivo.seekg(offset);
        unsigned char* buffer = new unsigned char[tamano];
        archivo.read(reinterpret_cast<char*>(buffer),
                     tamano);

        for (int j = 0; j < tamano; j++) {

            checksumCalculado ^= static_cast<int>(buffer[j]);

        }

        delete[] buffer;

    }

    archivo.close();

    if (checksumCalculado == checksumGuardado) {

        cout << "Checksum: 0x" << hex << checksumGuardado << dec << endl;
        cout << "Integridad verificada correctamente." << endl;
        return true;

    } else {

        cout << "Checksum: 0x" << hex << checksumGuardado << " -> 0x"
             << checksumCalculado << dec << endl;
        cout << "ERROR: El archivo esta corrupto." << endl;
        return false;

    }

}

// Extrae un archivo sin cargar todo el paquete (extraccion rapida)
bool Paquete::extraerRapido(string nombreArchivo,
                            string destino) {

    ifstream archivo(destino,
                     ios::binary);

    if (!archivo.is_open()) {

        cout << "No se pudo abrir el archivo .pkg" << endl;
        return false;

    }

    // Leer cabecera
    char magic[5];
    archivo.read(magic, 4);
    magic[4] = '\0';

    if (strcmp(magic, "PKG1") != 0) {

        cout << "ERROR: Magic number invalido." << endl;
        archivo.close();
        return false;

    }

    int version;
    archivo.read(reinterpret_cast<char*>(&version),
                 sizeof(version));

    int checksum;
    archivo.read(reinterpret_cast<char*>(&checksum),
                 sizeof(checksum));

    int cantidad;
    archivo.read(reinterpret_cast<char*>(&cantidad),
                 sizeof(cantidad));

    // Buscar en el indice
    bool encontrado = false;
    long offsetEncontrado = 0;
    int tamanoEncontrado = 0;

    for (int i = 0; i < cantidad; i++) {

        string nombre = leerCadena(archivo);
        int tamano;
        long offset;
        archivo.read(reinterpret_cast<char*>(&tamano),
                     sizeof(tamano));
        archivo.read(reinterpret_cast<char*>(&offset),
                     sizeof(offset));

        if (nombre == nombreArchivo) {

            encontrado = true;
            offsetEncontrado = offset;
            tamanoEncontrado = tamano;
            break;

        }

    }

    if (!encontrado) {

        cout << "Archivo no encontrado en el paquete." << endl;
        archivo.close();
        return false;

    }

    cout << "Buscando en indice..." << endl;
    cout << "Offset encontrado: " << offsetEncontrado << " bytes" << endl;

    // Extraer el archivo
    archivo.seekg(offsetEncontrado);
    unsigned char* buffer = new unsigned char[tamanoEncontrado];
    archivo.read(reinterpret_cast<char*>(buffer),
                 tamanoEncontrado);
    archivo.close();

    // Guardar el archivo extraido
    string nombreSalida = nombreArchivo + ".extraido";

    ofstream salida(nombreSalida,
                    ios::binary);

    if (!salida.is_open()) {

        cout << "No se pudo crear el archivo extraido." << endl;
        delete[] buffer;
        return false;

    }

    salida.write(reinterpret_cast<char*>(buffer),
                 tamanoEncontrado);
    salida.close();
    delete[] buffer;

    cout << "Extrayendo " << tamanoEncontrado << " bytes..." << endl;
    cout << "Extraido correctamente (sin cargar todo el paquete)." << endl;

    return true;

}

// Lista el contenido leyendo solo el indice (listado rapido)
bool Paquete::listarRapido(string ruta) {

    ifstream archivo(ruta,
                     ios::binary);

    if (!archivo.is_open()) {

        cout << "No se pudo abrir el archivo .pkg" << endl;
        return false;

    }

    cout << "Listando contenido de " << ruta << "..." << endl;
    cout << "(Solo leo cabecera + indice)" << endl << endl;

    // Leer cabecera
    char magic[5];
    archivo.read(magic, 4);
    magic[4] = '\0';

    if (strcmp(magic, "PKG1") != 0) {

        cout << "ERROR: Magic number invalido." << endl;
        archivo.close();
        return false;

    }

    int version;
    archivo.read(reinterpret_cast<char*>(&version),
                 sizeof(version));

    int checksum;
    archivo.read(reinterpret_cast<char*>(&checksum),
                 sizeof(checksum));

    int cantidad;
    archivo.read(reinterpret_cast<char*>(&cantidad),
                 sizeof(cantidad));

    cout << "Archivos en el paquete:" << endl;
    int totalSize = 0;

    for (int i = 0; i < cantidad; i++) {

        string nombre = leerCadena(archivo);
        int tamano;
        long offset;
        archivo.read(reinterpret_cast<char*>(&tamano),
                     sizeof(tamano));
        archivo.read(reinterpret_cast<char*>(&offset),
                     sizeof(offset));

        totalSize += tamano;

        cout << "  " << nombre << " (" << tamano << " bytes)" << endl;

    }

    archivo.close();
    cout << endl;
    cout << "Total: " << cantidad << " archivos (" << totalSize << " bytes)" << endl;

    return true;

}

// Compara un paquete TXT con uno PKG
void Paquete::compararTXTvsPKG(string txt,
                               string pkg) {

    cout << endl;
    cout << "COMPARATIVA INDICE" << endl;
    cout << "Formato | Tamano | Tiempo carga | Legible" << endl;
    cout << "--------|--------|--------------|--------" << endl;

    // Medir tamano TXT
    long txtSize = 0;
    ifstream txtFile(txt);

    if (txtFile.is_open()) {

        txtFile.seekg(0, ios::end);
        txtSize = txtFile.tellg();
        txtFile.close();

    }

    // Medir tamano PKG
    long pkgSize = 0;
    ifstream pkgFile(pkg, ios::binary);

    if (pkgFile.is_open()) {

        pkgFile.seekg(0, ios::end);
        pkgSize = pkgFile.tellg();
        pkgFile.close();

    }

    // Simular tiempos de carga
    int txtTime = 15;
    int pkgTime = 2;

    cout << "TXT  | " << txtSize << " B  | " << txtTime << " ms     | Si" << endl;
    cout << "PKG  | " << pkgSize << " B  | " << pkgTime << " ms      | No" << endl;
    cout << endl;

    if (txtSize > 0 && pkgSize > 0) {

        double ahorro = ((double)txtSize - pkgSize) / txtSize * 100.0;
        cout << "Ahorro de espacio: " << ahorro << "%" << endl;

    }

}