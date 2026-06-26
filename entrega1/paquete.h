#ifndef PAQUETE_H
#define PAQUETE_H

#include <string>
#include <vector>
#include <fstream>

struct Archivo {
    std::string nombre;
    unsigned char* contenido;
    int tamano;
    int checksum;
    bool comprimido;
    int tamanoOriginal;

    Archivo();
    Archivo(const Archivo& otro);
    Archivo& operator=(const Archivo& otro);
    ~Archivo();
};

class Paquete {
private:
    std::string nombre;
    std::vector<Archivo> archivos;

    int calcularChecksum(unsigned char* data, int size);

public:
    Paquete(std::string nombre);
    Paquete(const Paquete& otro);
    ~Paquete();
    Paquete& operator=(const Paquete& otro);

    // Entrega 1
    void agregarArchivo(std::string nombre, unsigned char* data, int size);
    void listarArchivos();
    void extraerArchivo(int indice);
    bool eliminarArchivo(std::string nombre);
    int buscarArchivo(std::string nombre);

    void comprimirArchivo(int indice);
    void descomprimirArchivo(int indice);
    bool verificarIntegridad(int indice);

    // Entrega 2
    bool agregarArchivoReal(std::string ruta);
    bool extraerArchivoDisco(int indice, std::string ruta);
    bool exportarIndice(std::string archivo);
    bool guardarPaqueteTXT(std::string archivo);
    bool cargarPaqueteTXT(std::string archivo);
    void mostrarInformacion();
//
    std::string getNombre();
    const std::vector<Archivo>& getArchivos() const;
};

#endif