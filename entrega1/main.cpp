#include <iostream>
#include <vector>
#include "paquete.h"

using namespace std;

void inicializarDatos(vector<Paquete>& paquetes);
void mostrarMenu();
void fusionarPaquetes(vector<Paquete>& paquetes);
void compararDuplicados(vector<Paquete>& paquetes);

int main() {
    vector<Paquete> paquetes;
    inicializarDatos(paquetes);

    int opcion;

    do {
        mostrarMenu();
        cout << "Opcion: ";
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(10000, ' ');
            opcion = 0;
};

        if (opcion == 1) {
            for (int i = 0; i < paquetes.size(); i++) {
                paquetes[i].listarArchivos();
            }
        }

        else if (opcion == 2) {
            int p, a;
            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {
                paquetes[p].listarArchivos();
                cout << "Archivo: ";
                cin >> a;
                paquetes[p].extraerArchivo(a);
            }
        }

        else if (opcion == 3) {
            int p;
            string n;

            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {
                cout << "Nombre archivo: ";
                cin >> n;
                paquetes[p].eliminarArchivo(n);
            }
        }

        else if (opcion == 4) {
            string n;
            cout << "Nombre: ";
            cin >> n;

            for (int i = 0; i < paquetes.size(); i++) {
                paquetes[i].buscarArchivo(n);
            }
        }

        else if (opcion == 5) {
            int p, a;
            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {
                paquetes[p].listarArchivos();
                cout << "Archivo: ";
                cin >> a;
                paquetes[p].comprimirArchivo(a);
            }
        }

        else if (opcion == 6) {
            int p, a;
            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {
                paquetes[p].listarArchivos();
                cout << "Archivo: ";
                cin >> a;
                paquetes[p].verificarIntegridad(a);
            }
        }

        else if (opcion == 7) {
            fusionarPaquetes(paquetes);
        }

        else if (opcion == 8) {
            compararDuplicados(paquetes);
        }

        else if (opcion == 9) {
            int p, a;
            cout << "Paquete: ";
            cin >> p;

            if (p >= 0 && p < paquetes.size()) {
                paquetes[p].listarArchivos();
                cout << "Archivo: ";
                cin >> a;
                paquetes[p].descomprimirArchivo(a);
            }
        }

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
        }

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
        }

    } while (opcion != 12);

    return 0;
}

//  INICIALIZACION 

void inicializarDatos(vector<Paquete>& paquetes) {
    Paquete doc("documentos");

    unsigned char txt[] = "Hola mundo";
    doc.agregarArchivo("doc.txt", txt, sizeof(txt) - 1);

    unsigned char datos[100];
    for (int i = 0; i < 100; i++) datos[i] = i;

    doc.agregarArchivo("datos.bin", datos, 100);

    paquetes.push_back(doc);

    Paquete res("respaldo");
    res.agregarArchivo("datos.bin", datos, 100);

    paquetes.push_back(res);
}

//  MENU 

void mostrarMenu() {
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " GESTOR DE PAQUETES " << endl;
    cout << " " << endl;

    cout << "1. Ver paquetes" << endl;
    cout << "2. Extraer archivo" << endl;
    cout << "3. Eliminar archivo" << endl;
    cout << "4. Buscar archivo" << endl;
    cout << "5. Comprimir archivo" << endl;
    cout << "6. Verificar integridad" << endl;
    cout << "7. Fusionar paquetes" << endl;
    cout << "8. Comparar duplicados" << endl;
    cout << "9. Descomprimir archivo" << endl;
    cout << "10. Agregar archivo real" << endl;
    cout << "11. Extraer archivo al disco" << endl;
    cout << "12. Salir" << endl;

    cout << " " << endl;

}

    void fusionarPaquetes(vector<Paquete>& paquetes) {
    int a, b;

    cout << "Primer paquete: ";
    cin >> a;

    cout << "Segundo paquete: ";
    cin >> b;

    if (a < 0 || b < 0 || a >= paquetes.size() || b >= paquetes.size()) {
        cout << "Indices invalidos";
        return;
    }

    Paquete nuevo("fusion");

    const vector<Archivo>& p1 = paquetes[a].getArchivos();
    const vector<Archivo>& p2 = paquetes[b].getArchivos();

    for (int i = 0; i < p1.size(); i++) {
        nuevo.agregarArchivo(p1[i].nombre, p1[i].contenido, p1[i].tamano);
    }

    for (int i = 0; i < p2.size(); i++) {
        nuevo.agregarArchivo(p2[i].nombre, p2[i].contenido, p2[i].tamano);
    }

    paquetes.push_back(nuevo);

    cout << "Paquetes fusionados";
}

void compararDuplicados(vector<Paquete>& paquetes) {
    cout << "Buscando duplicados...";

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
                            cout << "Duplicado: "
                                 << a[x].nombre
                                 << " en paquetes "
                                 << i << " y " << j << endl;
                        }
                    }
                }
            }
        }
    }
}