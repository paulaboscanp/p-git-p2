#include <iostream>
#include <vector>
#include "paquete.h"

using namespace std;

// Prototipos de funciones
void inicializarDatos(vector<Paquete>& paquetes);
void mostrarMenu();
void fusionarPaquetes(vector<Paquete>& paquetes);
void compararDuplicados(vector<Paquete>& paquetes);

int main() {

    vector<Paquete> paquetes;

    // Carga algunos archivos de ejemplo
    inicializarDatos(paquetes);

    int opcion;

    do {

        mostrarMenu();

        cout << "Opcion: ";

        if (!(cin >> opcion)) {

            cout << "Opcion invalida." << endl;

            cin.clear();
            cin.ignore(10000, '\n');

            opcion = 0;
        }

        // Mostrar todos los paquetes
        if (opcion == 1) {

            for (int i = 0; i < paquetes.size(); i++) {

                paquetes[i].listarArchivos();

            }

        }

        // Mostrar el contenido de un archivo
        else if (opcion == 2) {

            int p;
            int a;

            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {

                paquetes[p].listarArchivos();

                cout << "Archivo: ";
                cin >> a;

                paquetes[p].extraerArchivo(a);

            }
            else {

                cout << "Paquete invalido." << endl;

            }

        }

        // Eliminar un archivo
        else if (opcion == 3) {

            int p;
            string nombre;

            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {

                cout << "Nombre del archivo: ";
                cin >> nombre;

                paquetes[p].eliminarArchivo(nombre);

            }
            else {

                cout << "Paquete invalido." << endl;

            }

        }

        // Buscar un archivo
        else if (opcion == 4) {

            string nombre;

            cout << "Nombre: ";
            cin >> nombre;

            for (int i = 0; i < paquetes.size(); i++) {

                paquetes[i].buscarArchivo(nombre);

            }

        }

        // Comprimir un archivo
        else if (opcion == 5) {

            int p;
            int a;

            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {

                paquetes[p].listarArchivos();

                cout << "Archivo: ";
                cin >> a;

                paquetes[p].comprimirArchivo(a);

            }
            else {

                cout << "Paquete invalido." << endl;

            }

        }

        // Verificar integridad
        else if (opcion == 6) {

            int p;
            int a;

            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {

                paquetes[p].listarArchivos();

                cout << "Archivo: ";
                cin >> a;

                paquetes[p].verificarIntegridad(a);

            }
            else {

                cout << "Paquete invalido." << endl;

            }

        }

        // Fusionar paquetes
        else if (opcion == 7) {

            fusionarPaquetes(paquetes);

        }

        // Comparar archivos duplicados
        else if (opcion == 8) {

            compararDuplicados(paquetes);

        }

                // Descomprimir un archivo
        else if (opcion == 9) {

            int p;
            int a;

            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {

                paquetes[p].listarArchivos();

                cout << "Archivo: ";
                cin >> a;

                paquetes[p].descomprimirArchivo(a);

            }
            else {

                cout << "Paquete invalido." << endl;

            }

        }

        // Importar un archivo desde el disco
        else if (opcion == 10) {

            int p;
            string ruta;

            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {

                cout << "Ruta del archivo: ";
                cin >> ruta;

                paquetes[p].agregarArchivoReal(ruta);

            }
            else {

                cout << "Paquete invalido." << endl;

            }

        }

        // Extraer un archivo al disco
        else if (opcion == 11) {

            int p;
            int a;
            string ruta;

            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {

                paquetes[p].listarArchivos();

                cout << "Archivo: ";
                cin >> a;

                cout << "Ruta destino: ";
                cin >> ruta;

                paquetes[p].extraerArchivoDisco(a, ruta);

            }
            else {

                cout << "Paquete invalido." << endl;

            }

        }

        // Exportar el indice del paquete
        else if (opcion == 12) {

            int p;
            string archivo;

            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {

                cout << "Nombre del archivo: ";
                cin >> archivo;

                paquetes[p].exportarIndice(archivo);

            }
            else {

                cout << "Paquete invalido." << endl;

            }

        }

        // Mostrar informacion del paquete
        else if (opcion == 13) {

            int p;

            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {

                paquetes[p].mostrarInformacion();

            }
            else {

                cout << "Paquete invalido." << endl;

            }

        }

        // Guardar un paquete en un archivo TXT
        else if (opcion == 14) {

            int p;
            string ruta;

            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {

                cout << "Archivo TXT: ";
                cin >> ruta;

                paquetes[p].guardarPaqueteTXT(ruta);

            }
            else {

                cout << "Paquete invalido." << endl;

            }

        }

        // Cargar un paquete desde un archivo TXT
        else if (opcion == 15) {

            int p;
            string ruta;

            cout << "Paquete destino: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {

                cout << "Archivo TXT: ";
                cin >> ruta;

                paquetes[p].cargarPaqueteTXT(ruta);

            }
            else {

                cout << "Paquete invalido." << endl;

            }

        }

        // Importar archivo binario
    else if (opcion == 16) {

        int p;
        string ruta;

        cout << "Paquete: ";
        cin >> p;

        if (p >= 0 && p < paquetes.size()) {

            cout << "Ruta del archivo: ";
            cin >> ruta;

            paquetes[p].importarArchivoBinario(ruta);

        }
        else {

            cout << "Paquete invalido." << endl;

        }

    }

    // Exportar archivo binario
    else if (opcion == 17) {

        int p;
        int a;
        string ruta;

        cout << "Paquete: ";
        cin >> p;

        if (p >= 0 && p < paquetes.size()) {

            paquetes[p].listarArchivos();

            cout << "Archivo: ";
            cin >> a;

            cout << "Ruta destino: ";
            cin >> ruta;

            paquetes[p].exportarArchivoBinario(a,
                                            ruta);

        }
        else {

            cout << "Paquete invalido." << endl;

        }

    }

        // ENTREGA 3 - PAQUETES BINARIOS (.pkg)

        // Guardar paquete a .pkg
        else if (opcion == 18) {

            int p;
            string ruta;

            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {

                cout << "Nombre del archivo .pkg: ";
                cin >> ruta;

                paquetes[p].guardarPaquetePKG(ruta);

            }
            else {

                cout << "Paquete invalido." << endl;

            }

        }

        // Cargar paquete desde .pkg
        else if (opcion == 19) {

            int p;
            string ruta;

            cout << "Paquete destino: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {

                cout << "Archivo .pkg: ";
                cin >> ruta;

                paquetes[p].cargarPaquetePKG(ruta);

            }
            else {

                cout << "Paquete invalido." << endl;

            }

        }

        // Verificar integridad del .pkg
        else if (opcion == 20) {

            string ruta;

            cout << "Archivo .pkg: ";
            cin >> ruta;

            if (paquetes.size() > 0) {

                paquetes[0].verificarPKG(ruta);

            }
            else {

                cout << "No hay paquetes disponibles." << endl;

            }

        }

        // Extraer archivo rapido
        else if (opcion == 21) {

            string nombreArchivo;
            string ruta;

            cout << "Nombre del archivo a extraer: ";
            cin >> nombreArchivo;

            cout << "Ruta del .pkg: ";
            cin >> ruta;

            if (paquetes.size() > 0) {

                paquetes[0].extraerRapido(nombreArchivo, ruta);

            }
            else {

                cout << "No hay paquetes disponibles." << endl;

            }

        }

        // Listar contenido rapido
        else if (opcion == 22) {

            string ruta;

            cout << "Ruta del .pkg: ";
            cin >> ruta;

            if (paquetes.size() > 0) {

                paquetes[0].listarRapido(ruta);

            }
            else {

                cout << "No hay paquetes disponibles." << endl;

            }

        }

        // Comparar TXT vs PKG
        else if (opcion == 23) {

            string txt;
            string pkg;

            cout << "Archivo TXT: ";
            cin >> txt;

            cout << "Archivo PKG: ";
            cin >> pkg;

            if (paquetes.size() > 0) {

                paquetes[0].compararTXTvsPKG(txt, pkg);

            }
            else {

                cout << "No hay paquetes disponibles." << endl;

            }

        }

        // Salir
        else if (opcion == 24) {

            cout << endl;
            cout << "Hasta luego." << endl;

        }

        else {
            cout << "Opcion invalida." << endl;
        }

    } while (opcion != 24);

    return 0;
}

// Inicializa algunos archivos de ejemplo
void inicializarDatos(vector<Paquete>& paquetes) {

    Paquete documentos("documentos");

    unsigned char txt[] = "Hola mundo";

    documentos.agregarArchivo("doc.txt",
                              txt,
                              sizeof(txt) - 1);

    unsigned char datos[100];

    for (int i = 0; i < 100; i++) {

        datos[i] = i;

    }

    documentos.agregarArchivo("datos.bin",
                              datos,
                              100);

    paquetes.push_back(documentos);

    Paquete respaldo("respaldo");

    respaldo.agregarArchivo("datos.bin",
                            datos,
                            100);

    paquetes.push_back(respaldo);

}

// Muestra el menu principal
void mostrarMenu() {

    cout << endl;
    cout << " " << endl;
    cout << "      GESTOR DE PAQUETES" << endl;
    cout << " " << endl;

    cout << "1.  Ver paquetes" << endl;
    cout << "2.  Extraer archivo" << endl;
    cout << "3.  Eliminar archivo" << endl;
    cout << "4.  Buscar archivo" << endl;
    cout << "5.  Comprimir archivo" << endl;
    cout << "6.  Verificar integridad" << endl;
    cout << "7.  Fusionar paquetes" << endl;
    cout << "8.  Comparar duplicados" << endl;
    cout << "9.  Descomprimir archivo" << endl;
    cout << "10. Agregar archivo real" << endl;
    cout << "11. Extraer archivo al disco" << endl;
    cout << "12. Exportar indice" << endl;
    cout << "13. Ver informacion del paquete" << endl;
    cout << "14. Guardar paquete TXT" << endl;
    cout << "15. Cargar paquete TXT" << endl;
    cout << "16. Importar archivo binario" << endl;
    cout << "17. Exportar archivo binario" << endl;
    cout << "--- PAQUETES BINARIOS (.pkg) ---" << endl;
    cout << "18. Guardar paquete a .pkg" << endl;
    cout << "19. Cargar paquete desde .pkg" << endl;
    cout << "20. Verificar integridad del .pkg" << endl;
    cout << "21. Extraer archivo rapido" << endl;
    cout << "22. Listar contenido rapido" << endl;
    cout << "23. Comparar TXT vs PKG" << endl;
    cout << "24. Salir" << endl;

    cout << endl;

}

// Fusiona dos paquetes
void fusionarPaquetes(vector<Paquete>& paquetes) {

    int a;
    int b;

    cout << "Primer paquete: ";
    cin >> a;

    cout << "Segundo paquete: ";
    cin >> b;

    if (a < 0 || b < 0 ||
        a >= paquetes.size() ||
        b >= paquetes.size()) {

        cout << "Indices invalidos." << endl;
        return;

    }

    Paquete nuevo("fusion");

    const vector<Archivo>& p1 = paquetes[a].getArchivos();
    const vector<Archivo>& p2 = paquetes[b].getArchivos();

    for (int i = 0; i < p1.size(); i++) {

        nuevo.agregarArchivo(p1[i].nombre,
                             p1[i].contenido,
                             p1[i].tamano);

    }

    for (int i = 0; i < p2.size(); i++) {

        nuevo.agregarArchivo(p2[i].nombre,
                             p2[i].contenido,
                             p2[i].tamano);

    }

    paquetes.push_back(nuevo);

    cout << "Paquetes fusionados correctamente."
         << endl;

}

// Busca archivos duplicados entre paquetes
void compararDuplicados(vector<Paquete>& paquetes) {

    cout << endl;
    cout << "Buscando duplicados..." << endl;

    bool encontrado = false;

    for (int i = 0; i < paquetes.size(); i++) {

        for (int j = i + 1; j < paquetes.size(); j++) {

            const vector<Archivo>& a = paquetes[i].getArchivos();
            const vector<Archivo>& b = paquetes[j].getArchivos();

            for (int x = 0; x < a.size(); x++) {

                for (int y = 0; y < b.size(); y++) {

                    if (a[x].tamano == b[y].tamano) {

                        bool iguales = true;

                        for (int k = 0; k < a[x].tamano; k++) {

                            if (a[x].contenido[k] != b[y].contenido[k]) {

                                iguales = false;
                                break;

                            }

                        }

                        if (iguales) {

                            encontrado = true;

                            cout << "Duplicado: "
                                 << a[x].nombre
                                 << " entre paquetes "
                                 << i
                                 << " y "
                                 << j
                                 << endl;

                        }

                    }

                }

            }

        }

    }

    if (!encontrado) {

        cout << "No se encontraron archivos duplicados."
             << endl;

    }

}