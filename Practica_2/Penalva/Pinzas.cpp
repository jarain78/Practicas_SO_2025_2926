#include <iostream>  // Para entrada/salida estándar (cout, endl)
#include <fstream>   // Para manejo de archivos (ifstream, ofstream)
#include <cmath>     // Para funciones matemáticas (fabs)
#include <vector>    // Para contenedores dinámicos (vector)
#include <string>    // Para manejo de cadenas (string)

using namespace std;

// Programa principal para analizar datos de telemetría de una pinza robótica
int main() {
    // Abrir archivo de entrada con los datos de la pinza
    ifstream archivo("datos_pinza.txt");

    // Verificar si el archivo se abrió correctamente
    if (!archivo) {
        cout << "Error al abrir el archivo" << endl;
        return 1;  // Salir con código de error
    }

    // Vectores para almacenar los datos leídos
    vector<int> ids;           // IDs de las muestras
    vector<float> galga;       // Valores de la galga (sensor de fuerza)
    vector<float> fuerza_izq;  // Fuerza aplicada por la pinza izquierda
    vector<float> fuerza_der;  // Fuerza aplicada por la pinza derecha
    vector<string> estado;     // Clasificación de estabilidad (ESTABLE/INESTABLE)

    // Lectura de datos del archivo
    // Cada línea contiene: ID galga fuerza_izq fuerza_der
    int id;
    float g, fi, fd;
    while (archivo >> id >> g >> fi >> fd) {
        ids.push_back(id);        // Agregar ID al vector
        galga.push_back(g);       // Agregar valor de galga
        fuerza_izq.push_back(fi); // Agregar fuerza izquierda
        fuerza_der.push_back(fd); // Agregar fuerza derecha
    }

    archivo.close();  // Cerrar archivo de entrada

    size_t n = ids.size();  // Número total de muestras leídas

    cout << "Número de muestras leídas: " << n << endl;

    // Cálculo de medias aritméticas
    float suma_galga = 0, suma_izq = 0, suma_der = 0;

    for (size_t i = 0; i < n; i++) {
        suma_galga += galga[i];     // Suma de valores de galga
        suma_izq += fuerza_izq[i];  // Suma de fuerzas izquierdas
        suma_der += fuerza_der[i];  // Suma de fuerzas derechas
    }

    // Calcular medias dividiendo por el número de muestras
    float media_galga = suma_galga / n;
    float media_izq = suma_izq / n;
    float media_der = suma_der / n;

    // Mostrar las medias por pantalla (solo esto se imprime en consola)
    cout << "Media galga: " << media_galga << endl;
    cout << "Media fuerza izquierda: " << media_izq << endl;
    cout << "Media fuerza derecha: " << media_der << endl;

    // Evaluación de estabilidad para cada muestra
    // Si la diferencia absoluta entre fuerzas > 0.15, es INESTABLE
    for (size_t i = 0; i < n; i++) {
        float diferencia = fabs(fuerza_izq[i] - fuerza_der[i]);

        if (diferencia > 0.15)
            estado.push_back("INESTABLE");  // Agregar clasificación inestable
        else
            estado.push_back("ESTABLE");    // Agregar clasificación estable
    }

    // Escritura de resultados en archivo de salida
    ofstream salida("resultado_pinza.txt");

    // Encabezado del archivo
    salida << "RESULTADOS DEL SISTEMA DE PINZA ROBOTICA\n\n";

    // Sección de datos originales
    salida << "Datos:\n";
    for (size_t i = 0; i < n; i++) {
        salida << ids[i] << " "
               << galga[i] << " "
               << fuerza_izq[i] << " "
               << fuerza_der[i] << endl;
    }

    // Sección de medias calculadas
    salida << "\nMedias:\n";
    salida << "Galga = " << media_galga << endl;
    salida << "Fuerza izquierda = " << media_izq << endl;
    salida << "Fuerza derecha = " << media_der << endl;

    // Sección de clasificación de estabilidad
    salida << "\nClasificacion:\n";
    for (size_t i = 0; i < n; i++) {
        salida << ids[i] << " " << estado[i] << endl;
    }

    // Verificar si no hay datos (archivo vacío)
    if (n == 0) {
        cout << "No hay datos en el archivo." << endl;
        return 1;  // Salir con error
    }

    salida.close();  // Cerrar archivo de salida

    return 0;  // Fin exitoso del programa
}