#include <iostream>
using namespace std;


void leer_matriz();

int suma = 0;
int contadorMenores = 0;
int filas = 6;
int columnas = 6;

int matriz[6][6] = {
    {21, 23, 25, 24, 22, 20},
    {19, 18, 21, 23, 22, 24},
    {26, 27, 25, 24, 23, 22},
    {20, 19, 18, 21, 22, 23},
    {24, 25, 26, 27, 28, 29},
    {22, 21, 20, 19, 18, 17}
};


int main() {

    leer_matriz();  
    double promedio = static_cast<double>(suma) / (filas * columnas);

    cout << "Temperatura promedio: " << promedio << endl;
    cout << "Cantidad de valores inferiores a 22: " << contadorMenores << endl;

    return 0;
}


void leer_matriz() {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            suma += matriz[i][j];

            if (matriz[i][j] < 22) {
                contadorMenores++;
            }
        }
    }
}