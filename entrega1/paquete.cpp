#include "paquete.h"
#include <iostream>
#include <cstring>

using namespace std;

Paquete::Paquete(string nombre) {
    this->nombre = nombre;

}

Paquete::Paquete(const Paquete& otro) {
    nombre = otro.nombre;
    for (int i = 0; i < otro.archivos.size(); i++) {
        Archivo nuevo;
        nuevo.nombre = otro.archivos[i].nombre;
        nuevo.tamano = otro.archivos[i].tamano;
        nuevo.contenido = new unsigned char[nuevo.tamano];
        memcpy(nuevo.contenido, otro.archivos[i].contenido, nuevo.tamano);
        nuevo.checksum = otro.archivos[i].checksum;
        archivos.push_back(nuevo);
    }
}

Paquete::~Paquete() {
    for (int i = 0; i < archivos.size(); i++) {
        delete[] archivos[i].contenido;
    }
}

Paquete& Paquete::operator=(const Paquete& otro) {
    if (this != &otro) {
        for (int i = 0; i < archivos.size(); i++) {
            delete[] archivos[i].contenido;
        }

        archivos.clear();
        for (int i = 0; i < otro.archivos.size(); i++) {
            Archivo nuevo;
            nuevo.nombre = otro.archivos[i].nombre;
            nuevo.tamano = otro.archivos[i].tamano;
            nuevo.contenido = new unsigned char[nuevo.tamano];
            memcpy(nuevo.contenido, otro.archivos[i].contenido, nuevo.tamano);
            nuevo.checksum = otro.archivos[i].checksum;
            archivos.push_back(nuevo);
        }
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
    Archivo nuevo;
    nuevo.nombre = nombre;
    nuevo.tamano = size;
    nuevo.contenido = new unsigned char[size];
    memcpy(nuevo.contenido, data, size);
    nuevo.checksum = calcularChecksum(data, size);
    archivos.push_back(nuevo);
    cout << "Archivo '" << nombre << "' agregado a '" << this->nombre << "'" << endl;
}

void Paquete::extraerArchivo(int indice) {
    if (indice < 0 || indice >= archivos.size()){
        cout << "Indice invalido" << endl;
        return;
    }

    cout << "Contenido de '" << archivos[indice].nombre << "'  " << endl;
    cout << "Tamano: " << archivos[indice].tamano << " bytes" << endl;
    cout << "Checksum: " << archivos[indice].checksum << endl;
    cout << "Contenido: ";

    for (int i = 0; i < archivos[indice].tamano && i < 200; i++) {
        if (archivos[indice].contenido[i] >= 32 && archivos[indice].contenido[i] <= 126) {
            cout << archivos[indice].contenido[i];
        } else {
            cout << "[" << (int)archivos[indice].contenido[i] << "]";
        }
    }

    if (archivos[indice].tamano > 200) cout << "...";
    cout << endl;
}

bool Paquete::eliminarArchivo(string nombre) {
    for (int i = 0; i < archivos.size(); i++) {
        if (archivos[i].nombre == nombre) {
            delete[] archivos[i].contenido;
            archivos.erase(archivos.begin() + i);
            cout << "Archivo '" << nombre << "' eliminado" << endl;
            return true;
        }
    }            

    cout << "Archivo '" << nombre << "' no encontrado" << endl;
    return false;
}

int Paquete::buscarArchivo(string nombre) {
    for (int i = 0; i < archivos.size(); i++) {
        if (archivos[i].nombre == nombre) {
           cout << "Archivo '" << nombre << "' encontrado en indice " << i << endl;
            return i;
        }
    }

    cout << "Archivo '" << nombre << "' no encontrado" << endl;
    return -1;
}

void Paquete::comprimirArchivo(int indice) {
    if (indice < 0 || indice >= archivos.size()) return;

    Archivo& a = archivos[indice];
    int originalSize = a.tamano;

    unsigned char* comprimido = new unsigned char[originalSize * 2];
    int compIndex = 0;

    for (int i = 0; i < originalSize; i++) {
        int count = 1;
        while (i + 1 < originalSize && a.contenido[i] == a.contenido[i + 1] && count < 255) {
            count++;
            i++;
        }

        comprimido[compIndex++] = a.contenido[i];
        comprimido[compIndex++] = count;
    }

    int compSize = compIndex;
    double ratio = (double)compSize / originalSize * 100;

    cout << "Compresion RLE:" << endl;
    cout << " Original:  " << originalSize << " bytes" << endl;
    cout << " Comprimido: " << compSize << " bytes" << endl;
    cout << " Ratio: " << ratio << " % del original" << endl;

    delete[] comprimido;
}

bool Paquete::verificarIntegridad(int indice) {
    if (indice < 0 || indice >= archivos.size()) {
        cout << "Indice invalido" << endl;
        return false;
    }

    Archivo& a = archivos[indice];
    int checksumActual = calcularChecksum(a.contenido, a.tamano);

    cout << "Verificando integridad de '" << a.nombre << "':" << endl;
    cout << " Checksum almacenado: " << a.checksum << endl;
    cout << " Checksum calculado: " << checksumActual << endl;

    if (a.checksum == checksumActual) {
        cout << " INTEGRO" << endl;
        return true;
    } else {
        cout << " CORRUPTO" << endl;
        return false;
    }
}

string Paquete::getNombre() {
    return nombre;
}

vector<Archivo> Paquete::getArchivos() {
    return archivos;
}
    

    

