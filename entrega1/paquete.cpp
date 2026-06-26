#include "paquete.h"
#include <iostream>
#include <cstring>

using namespace std;

//  ARCHIVO 

Archivo::Archivo() {
    nombre = "";
    contenido = nullptr;
    tamano = 0;
    checksum = 0;
    comprimido = false;
    tamanoOriginal = 0;
}

Archivo::Archivo(const Archivo& otro) {
    nombre = otro.nombre;
    tamano = otro.tamano;
    checksum = otro.checksum;
    comprimido = otro.comprimido;
    tamanoOriginal = otro.tamanoOriginal;

    contenido = new unsigned char[tamano];
    memcpy(contenido, otro.contenido, tamano);
}

Archivo& Archivo::operator=(const Archivo& otro) {
    if (this != &otro) {
        delete[] contenido;

        nombre = otro.nombre;
        tamano = otro.tamano;
        checksum = otro.checksum;
        comprimido = otro.comprimido;
        tamanoOriginal = otro.tamanoOriginal;

        contenido = new unsigned char[tamano];
        memcpy(contenido, otro.contenido, tamano);
    }
    return *this;
}

Archivo::~Archivo() {
    delete[] contenido;
}

//  PAQUETE 

Paquete::Paquete(string nombre) {
    this->nombre = nombre;
}

Paquete::Paquete(const Paquete& otro) {
    nombre = otro.nombre;
    archivos = otro.archivos;
}

Paquete::~Paquete() {}

Paquete& Paquete::operator=(const Paquete& otro) {
    if (this != &otro) {
        nombre = otro.nombre;
        archivos = otro.archivos;
    }
    return *this;
}

int Paquete::calcularChecksum(unsigned char* data, int size) {
    int suma = 0;
    for (int i = 0; i < size; i++) {
        suma += data[i];
    }
    return suma;
}

void Paquete::agregarArchivo(string nombre, unsigned char* data, int size) {
    Archivo a;

    a.nombre = nombre;
    a.tamano = size;
    a.tamanoOriginal = size;
    a.comprimido = false;

    a.contenido = new unsigned char[size];
    memcpy(a.contenido, data, size);

    a.checksum = calcularChecksum(data, size);

    archivos.push_back(a);

    cout << "Archivo agregado: " << nombre << endl;
}

void Paquete::listarArchivos() {
    cout << "Paquete: " << nombre << endl;

    for (int i = 0; i < archivos.size(); i++) {
        cout << "[" << i << "] " << archivos[i].nombre
             << " (" << archivos[i].tamano << " bytes)";

        if (archivos[i].comprimido)
            cout << " [COMPRIMIDO]";

        cout << endl;
    }
}

void Paquete::extraerArchivo(int indice) {
    if (indice < 0 || indice >= archivos.size()) return;

    Archivo& a = archivos[indice];

    cout << "Archivo: " << a.nombre << endl;
    cout << "Tamano: " << a.tamano << endl;
    cout << "Checksum: " << a.checksum << endl;

    cout << "Contenido: ";

    for (int i = 0; i < a.tamano; i++) {
        cout << (int)a.contenido[i] << " ";
    }

    cout << endl;
}

bool Paquete::eliminarArchivo(string nombre) {
    for (int i = 0; i < archivos.size(); i++) {
        if (archivos[i].nombre == nombre) {
            archivos.erase(archivos.begin() + i);
            cout << "Archivo eliminado";
            return true;
        }
    }
    return false;
}

int Paquete::buscarArchivo(string nombre) {
    for (int i = 0; i < archivos.size(); i++) {
        if (archivos[i].nombre == nombre) {
            cout << "Encontrado en indice " << i << endl;
            return i;
        }
    }
    cout << "No encontrado";
    return -1;
}

//  COMPRESION 

void Paquete::comprimirArchivo(int indice) {
    if (indice < 0 || indice >= archivos.size()) return;

    Archivo& a = archivos[indice];

    if (a.comprimido) return;

    unsigned char* temp = new unsigned char[a.tamano * 2];
    int j = 0;

    for (int i = 0; i < a.tamano; i++) {
        int count = 1;

        while (i + 1 < a.tamano && a.contenido[i] == a.contenido[i + 1] && count < 255) {
            count++;
            i++;
        }

        temp[j++] = a.contenido[i];
        temp[j++] = count;
    }

    delete[] a.contenido;

    a.contenido = new unsigned char[j];
    memcpy(a.contenido, temp, j);

    a.tamano = j;
    a.comprimido = true;

    delete[] temp;

    cout << "Archivo comprimido";
}

//  DESCOMPRESION

void Paquete::descomprimirArchivo(int indice) {
    if (indice < 0 || indice >= archivos.size()) return;

    Archivo& a = archivos[indice];

    if (!a.comprimido) return;

    unsigned char* temp = new unsigned char[a.tamanoOriginal];

    int j = 0;

    for (int i = 0; i < a.tamano; i += 2) {
        unsigned char val = a.contenido[i];
        unsigned char count = a.contenido[i + 1];

        for (int k = 0; k < count; k++) {
            temp[j++] = val;
        }
    }

    delete[] a.contenido;

    a.contenido = new unsigned char[a.tamanoOriginal];
    memcpy(a.contenido, temp, a.tamanoOriginal);

    a.tamano = a.tamanoOriginal;
    a.comprimido = false;

    delete[] temp;

    cout << "Archivo descomprimido";
}

// INTEGRIDAD 

bool Paquete::verificarIntegridad(int indice) {
    if (indice < 0 || indice >= archivos.size()) return false;

    Archivo& a = archivos[indice];

    int calc = calcularChecksum(a.contenido, a.tamano);

    cout << "Checksum esperado: " << a.checksum << endl;
    cout << "Checksum actual: " << calc << endl;

    return calc == a.checksum;
}

string Paquete::getNombre() {
    return nombre;
}

const vector<Archivo>& Paquete::getArchivos() const {
    return archivos;
}

bool Paquete::agregarArchivoReal(string ruta) {

    ifstream archivo(ruta);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo" << endl;
        return false;
    }

    string contenido;
    string linea;

    while (getline(archivo, linea)) {
        contenido += linea;
        contenido += "\n";
    }

    archivo.close();

    unsigned char* buffer =
        new unsigned char[contenido.size()];

    memcpy(buffer,
           contenido.c_str(),
           contenido.size());

    agregarArchivo(ruta,
                   buffer,
                   contenido.size());

    delete[] buffer;

    cout << "Archivo importado correctamente" << endl;

    return true;
}

bool Paquete::extraerArchivoDisco(int indice,
                                  string ruta) {

    if (indice < 0 || indice >= archivos.size()) {
        return false;
    }

    ofstream archivo(ruta);

    if (!archivo.is_open()) {
        cout << "No se pudo crear el archivo" << endl;
        return false;
    }

    Archivo& a = archivos[indice];

    for (int i = 0; i < a.tamano; i++) {
        archivo << (char)a.contenido[i];
    }

    archivo.close();

    cout << "Archivo extraido correctamente" << endl;

    return true;
}

void Paquete::mostrarInformacion() {

    int totalBytes = 0;

    for (int i = 0; i < archivos.size(); i++) {
        totalBytes += archivos[i].tamano;
    }

    cout << "INFORMACION DEL PAQUETE" << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Cantidad de archivos: " << archivos.size() << endl;
    cout << "Tamano total: " << totalBytes << " bytes" << endl;
}

bool Paquete::exportarIndice(string archivoSalida) {

    ofstream archivo(archivoSalida);

    if (!archivo.is_open()) {
        cout << "No se pudo crear el archivo" << endl;
        return false;
    }

    archivo << "Nombre,Tamano,Checksum\n";

    for (int i = 0; i < archivos.size(); i++) {

        archivo << archivos[i].nombre << ","
                << archivos[i].tamano << ","
                << archivos[i].checksum
                << "\n";
    }

    archivo.close();

    cout << "Indice exportado correctamente" << endl;

    return true;
}

bool Paquete::guardarPaqueteTXT(string ruta) {
    ofstream archivo(ruta);

    if (!archivo.is_open()) {
        cout << "No se pudo crear el archivo" << endl;
        return false;
    }

    archivo << nombre << endl;
    archivo << archivos.size() << endl;

    for (int i = 0; i < archivos.size(); i++) {
        archivo << archivos[i].nombre << endl;
        archivo << archivos[i].tamano << endl;

        for (int j = 0; j < archivos[i].tamano; j++) {
            archivo << (int)archivos[i].contenido[j] << " ";
        }

        archivo << endl;
    }

    archivo.close();
    cout << "Paquete guardado correctamente" << endl;
    return true;
}

bool Paquete::cargarPaqueteTXT(string ruta) {
    ifstream archivo(ruta);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo" << endl;
        return false;
    }

    archivos.clear();

    getline(archivo, nombre);

    int cantidad;
    archivo >> cantidad;
    archivo.ignore();

    for (int i = 0; i < cantidad; i++) {
        string nombreArchivo;
        getline(archivo, nombreArchivo);

        int tamano;
        archivo >> tamano;

        unsigned char* buffer =
            new unsigned char[tamano];
        for (int j = 0; j < tamano; j++) {
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
    cout << "Paquete cargado correctamente" << endl;
    return true;
}