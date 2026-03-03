#include <iostream>
using namespace std;

void calcularEnergia(float& v, float& t, float& eficiencia) {
    cout << "Velocidad del viento (m/s): ";
    cin >> v;
    cout << "Horas de funcionamiento: ";
    cin >> t;
    cout << "Eficiencia (0 - 1): ";
    cin >> eficiencia;
}

int main() {
    float P = 1.225;
    float A = 50;
    float v, t, eficiencia;

    calcularEnergia(v, t, eficiencia);

    float energia = 0.5 * P * A * v * v * v * t * eficiencia;

    cout << "Energia generada: " << energia << " kWh" << endl;

    return 0;
}


//g++ -Wall -Wextra -g3 -O0 Examen_3/JaunManuel.cpp -o build/Debug/outDebug

/*g++ -Wall -Wextra -g3 -O0 \
  Examen_3/JaunManuel.cpp \
  -o build/Debug/outDebug*/