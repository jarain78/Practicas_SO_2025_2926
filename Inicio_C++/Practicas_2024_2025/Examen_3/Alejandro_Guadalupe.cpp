#include <iostream>
using namespace std;

// Función para calcular la energía
float calcularEnergia(float v, float t, float eficiencia) {
    float densidad = 1.225; // kg/m^3
    float area = 50;        // m^2

    float energia = 0.5 * densidad * area * v * v * v * t * eficiencia;

    return energia;
}

int main() {
    float velocidad;
    float tiempo;
    float eficiencia;

    cout << "Introduce la velocidad del viento (m/s): ";
    cin >> velocidad;

    cout << "Introduce el tiempo de funcionamiento (h): ";
    cin >> tiempo;

    cout << "Introduce la eficiencia (0 - 1): ";
    cin >> eficiencia;

    if (eficiencia < 0 || eficiencia > 1) {
        cout << "Error: La eficiencia debe estar entre 0 y 1." << endl;
        return 1;
    }

    float energiaGenerada = calcularEnergia(velocidad, tiempo, eficiencia);

    cout << "La energia generada es: " << energiaGenerada << " kWh" << endl;

    return 0;
}

/*g++ -Wall -Wextra -g3 -O0 \
  Examen_3/Alejandro_Guadalupe.cpp \
  -o build/Debug/outDebug*/