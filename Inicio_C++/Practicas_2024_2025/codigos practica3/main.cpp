#include <iostream>            
#include "clientrobotunity.h" //importa el archivo .h

using namespace std;

int main() {
    int opcion; //declaramos la variable opción, siendo un número entero (1 o 2)

    clientrobotunity client("http://localhost:8080"); //crea el cliente

    do { // iniciamos bucle que se repite hasta que salgamos del programa
        cout << "\n===== MENÚ =====\n"; //creamos un menu para que el cliente escoga que quiere hacer
        cout << "1) Mostrar datos  (objeto, LIDAR, imágen)\n";// muestra los datos de las posiciones y la imágen
        cout << "2) Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion; // pedimos al cliente que escoja una opción

        if (opcion == 1) { // si se escoge 1, se ejecuta esta parte
            cout << "\n Obteniendo los datos de puntos\n";
            client.send_points();

            cout << "\n Obteniendo la imágen \n";
            client.get_image();

            cout << "\n Obteniendo datos del LIDAR \n";
            client.get_lidar();

        } else if (opcion == 2) {
            cout << "\n Saliendo del programa...\n";
        } else { // se muestra el escoger opción hasta que se marque una correcta
            cout << "\n Opción no válida. Intentelo de nuevo:\n";
        }

    } while (opcion != 2); // se repite el bucle hasta que se marque 2= salir

    return 0; // indica el fin del programa
}
