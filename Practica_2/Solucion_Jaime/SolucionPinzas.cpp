#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

const int MAX_DATOS = 100;
const float UMBRAL = 0.15;

int main() {
    int ids[MAX_DATOS];

    // Matriz con las 3 variables de sensores:
    // columna 0 = galga
    // columna 1 = fuerza izquierda
    // columna 2 = fuerza derecha
    float datos[MAX_DATOS][3];

    float galga[MAX_DATOS];
    float fuerza_izq[MAX_DATOS];
    float fuerza_der[MAX_DATOS];

    int n = 0;

    ifstream archivoEntrada("datos_pinza.txt");

    if (!archivoEntrada.is_open()) {
        cout << "Error: no se pudo abrir el archivo datos_pinza.txt" << endl;
        return 1;
    }

    while (n < MAX_DATOS &&
           archivoEntrada >> ids[n] >> galga[n] >> fuerza_izq[n] >> fuerza_der[n]) {

        datos[n][0] = galga[n];
        datos[n][1] = fuerza_izq[n];
        datos[n][2] = fuerza_der[n];

        n++;
    }

    archivoEntrada.close();

    if (n == 0) {
        cout << "Error: el archivo no contiene datos validos." << endl;
        return 1;
    }

    float sumaGalga = 0;
    float sumaIzq = 0;
    float sumaDer = 0;

    cout << fixed << setprecision(3);

    cout << "DATOS LEIDOS:" << endl;

    for (int i = 0; i < n; i++) {
        sumaGalga += galga[i];
        sumaIzq += fuerza_izq[i];
        sumaDer += fuerza_der[i];

        cout << "ID: " << ids[i]
             << " Galga: " << galga[i]
             << " Izq: " << fuerza_izq[i]
             << " Der: " << fuerza_der[i]
             << endl;
    }

    float mediaGalga = sumaGalga / n;
    float mediaIzq = sumaIzq / n;
    float mediaDer = sumaDer / n;

    cout << endl;
    cout << "Media galga: " << mediaGalga << endl;
    cout << "Media fuerza izquierda: " << mediaIzq << endl;
    cout << "Media fuerza derecha: " << mediaDer << endl;

    ofstream archivoSalida("resultado_pinza.txt");

    if (!archivoSalida.is_open()) {
        cout << "Error: no se pudo crear resultado_pinza.txt" << endl;
        return 1;
    }

    archivoSalida << fixed << setprecision(3);

    archivoSalida << "RESULTADOS DEL SISTEMA DE PINZA ROBOTICA" << endl;
    archivoSalida << "Numero de muestras leidas: " << n << endl;
    archivoSalida << endl;

    archivoSalida << "Datos:" << endl;

    for (int i = 0; i < n; i++) {
        archivoSalida << ids[i] << " "
                      << datos[i][0] << " "
                      << datos[i][1] << " "
                      << datos[i][2] << endl;
    }

    archivoSalida << endl;
    archivoSalida << "Medias:" << endl;
    archivoSalida << "Galga = " << mediaGalga << endl;
    archivoSalida << "Fuerza izquierda = " << mediaIzq << endl;
    archivoSalida << "Fuerza derecha = " << mediaDer << endl;

    archivoSalida << endl;
    archivoSalida << "Clasificacion:" << endl;

    for (int i = 0; i < n; i++) {
        float diferencia = fabs(fuerza_izq[i] - fuerza_der[i]);

        if (diferencia > UMBRAL) {
            archivoSalida << ids[i] << " INESTABLE" << endl;
            cout << "Estado muestra " << ids[i] << ": INESTABLE" << endl;
        } else {
            archivoSalida << ids[i] << " ESTABLE" << endl;
            cout << "Estado muestra " << ids[i] << ": ESTABLE" << endl;
        }
    }

    archivoSalida.close();

    cout << endl;
    cout << "Archivo resultado_pinza.txt generado correctamente." << endl;

    return 0;
}