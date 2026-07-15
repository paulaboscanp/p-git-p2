#ifndef PAQUETE_H
#define PAQUETE_H

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cstdint>

// Representa un archivo almacenado dentro del paquete
struct Archivo {

    // Nombre del archivo
    std::string nombre;

    // Contenido almacenado en memoria dinamica
    unsigned char* contenido;

    // Tamano actual del archivo
    int tamano;

    // Checksum utilizado para verificar integridad
    int checksum;

    // Indica si el archivo esta comprimido
    bool comprimido;

    // Tamano antes de comprimir
    int tamanoOriginal;

    Archivo();
    Archivo(const Archivo& otro);
    Archivo& operator=(const Archivo& otro);
    ~Archivo();
};

// Entrada del indice binario utilizada en archivos .pkg
struct EntradaIndice {

    // Nombre del archivo
    std::string nombre;

    // Tamano del archivo
    int tamano;

    // Posicion donde comienza dentro del .pkg
    long offset;
};

// Clase que administra un paquete de archivos
class Paquete {

private:

    // Nombre del paquete
    std::string nombre;

    // Lista de archivos almacenados
    std::vector<Archivo> archivos;

    // Calcula el checksum del contenido
    int calcularChecksum(unsigned char* data, int size);

    // Calcula el checksum para archivos binarios
    int calcularChecksumBinario(unsigned char* data, int size);

    // Escribe una cadena en formato binario
    void escribirCadena(std::ofstream& archivo,
                        const std::string& texto);

    // Lee una cadena desde un archivo binario
    std::string leerCadena(std::ifstream& archivo);

public:

    // Constructores y destructor
    Paquete(std::string nombre);
    Paquete(const Paquete& otro);
    ~Paquete();

    // Operador de asignacion
    Paquete& operator=(const Paquete& otro);

    // FUNCIONES ENTREGA 1

    void agregarArchivo(std::string nombre,
                        unsigned char* data,
                        int size);

    void listarArchivos();

    void extraerArchivo(int indice);

    bool eliminarArchivo(std::string nombre);

    int buscarArchivo(std::string nombre);

    void comprimirArchivo(int indice);

    void descomprimirArchivo(int indice);

    bool verificarIntegridad(int indice);

    // FUNCIONES ENTREGA 2

    // Importa un archivo de texto desde el disco
    bool agregarArchivoReal(std::string ruta);

    // Extrae un archivo del paquete al disco
    bool extraerArchivoDisco(int indice,
                             std::string ruta);

    // Exporta el indice del paquete
    bool exportarIndice(std::string archivo);

    // Guarda el paquete en formato texto
    bool guardarPaqueteTXT(std::string archivo);

    // Carga un paquete desde un TXT
    bool cargarPaqueteTXT(std::string archivo);

    // Muestra informacion resumida
    void mostrarInformacion();

    // FUNCIONES ENTREGA 3

    // Importa cualquier archivo usando modo binario
    bool importarArchivoBinario(std::string ruta);

    // Exporta un archivo binario al disco
    bool exportarArchivoBinario(int indice,
                                std::string ruta);

    // Guarda el paquete completo en formato .pkg
    bool guardarPaquetePKG(std::string ruta);

    // Carga un paquete desde un archivo .pkg
    bool cargarPaquetePKG(std::string ruta);

    // Verifica la integridad del archivo .pkg
    bool verificarPKG(std::string ruta);

    // Extrae un archivo sin cargar todo el paquete
    bool extraerRapido(std::string nombreArchivo,
                       std::string destino);

    // Lista el contenido leyendo solo el indice
    bool listarRapido(std::string ruta);

    // Compara un paquete TXT con uno PKG
    void compararTXTvsPKG(std::string txt,
                          std::string pkg);

    // FUNCIONES DE ACCESO

    std::string getNombre();

    const std::vector<Archivo>& getArchivos() const;
};

#endif