#include <iostream>
using namespace std;
#include <string>

float potencia(float base, float exponente) {
    int resultado = 1;
    for (int i = 0; i < exponente; i++) {
        resultado = base * resultado;
    }
    return resultado;
}

float calcularEnergia(float v, float t, float e, float p = 1.225, float A = 50) {
    float V_cubo = potencia(v, 3);
    return (0.5 * p * A * V_cubo * t * e);
}



int main() {
    int velocidad, tiempo, eficiencia = 0;
    cout << "Introduce respectivamente la velocidad (m/s) y el tiempo (horas)";
    cin >> velocidad;
    cin >> tiempo;
    cout << "Introduce la eficiencia (0-1)" << endl;
    while (eficiencia < 0 || eficiencia > 1) {
        cout << "No sirve ese valor";
    }
    cin >> eficiencia;
    
    cout << "La velocidad es: " << velocidad << " m/s" << endl;
    cout << "Tiempo: " << tiempo << " horas" << endl;
    cout << "Eficiencia: " << eficiencia << endl;
    float energia = calcularEnergia(velocidad, tiempo, eficiencia);
    cout << "La energia generada es: " << energia << " kWh" << endl;
}

/*g++ -Wall -Wextra -g3 -O0 \
  Examen_3/Esther.cpp \
  -o build/Debug/outDebug*/