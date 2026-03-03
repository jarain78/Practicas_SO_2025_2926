#include <iostream>
using namespace std;

float calcularEnergia(float v, float t, float eficiencia) {
    float P = 1.225;  // Densidad del aire (kg/m^3)
    float A = 60;     // Área de captación (m^2)

    float energia = 0.5 * P * A * v * v * v * t * eficiencia;

    cout << "La energía estimada es: " << energia << " kWh" << endl;

    return energia;
}

int main() {
    float v, t, eficiencia;

    cout << "Introduce la velocidad del viento (m/s): ";
    cin >> v;
    cout << "Introduce el tiempo de funcionamiento (h): ";
    cin >> t;
    cout << "Introduce la eficiencia (0-1): ";
    cin >> eficiencia;

    if (eficiencia < 0 || eficiencia > 1) {
        cout << "Eficiencia fuera de rango." << endl;
        return 1;
    }

    float energia = calcularEnergia(v, t, eficiencia);

    cout << "La energía generada será: " << energia << " kWh" << endl;

    return 0;
}

/*g++ -Wall -Wextra -g3 -O0 \
  Examen_3/JaunManuel.cpp \
  -o build/Debug/outDebug*/