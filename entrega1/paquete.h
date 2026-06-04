#ifndef PAQUETE_H
#define PAQUETE_H

#include <string>
#include <vector>

struct Archivo {
    std::string nombre;
    unsigned char* contenido;
    int tamaño;
    int checksum;
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

        void agregarArchivo(std::string nombre, unsigned char* data, int size);
        void listarArchivos();
        void extraerArchivo(int indice);
        bool eliminarArchivo(std::string nombre);
        int buscarArchivo(std::string nombre);
        void comprimirArchivo(int indice);
        void descomprimirArchivo(int indice);
        bool verificarIntegridad(int indice);
        std::string getNombre();
        std::vector<Archivo> getArchivos();
};

#endif