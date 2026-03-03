
#include <iostream>
#include <fcntl.h>      // Para open()
#include <unistd.h>     // Para read(), write(), close()
#include <termios.h>    // Para la configuración del puerto serie
#include <cstring>      // Para memset()

using namespace std;

int main() {
    const char* port = "/dev/tty.usbmodem1101";  // Cambia según el puerto del Arduino (/dev/ttyUSB0 o /dev/ttyACM0)
    int serialPort = open(port, O_RDWR | O_NOCTTY | O_NDELAY);

    if (serialPort == -1) {
        cerr << "Error al abrir el puerto serie: " << port << endl;
        return 1;
    }

    // Configuración del puerto serie
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(serialPort, &tty) != 0) {
        cerr << "Error al obtener los atributos del puerto." << endl;
        close(serialPort);
        return 1;
    }

    cfsetispeed(&tty, B9600);  // Velocidad en baudios (9600)
    cfsetospeed(&tty, B9600);

    tty.c_cflag = CS8 | CREAD | CLOCAL;  // 8 bits de datos, habilitar lectura, ignorar señales de control
    tty.c_iflag = IGNPAR;  // Ignorar errores de paridad
    tty.c_oflag = 0;
    tty.c_lflag = 0;  // Modo sin procesamiento de líneas

    tcflush(serialPort, TCIFLUSH);
    tcsetattr(serialPort, TCSANOW, &tty);

    char buffer[256];
    int bytesRead;

    cout << "Esperando datos del Arduino..." << endl;

    while (true) {
        bytesRead = read(serialPort, buffer, sizeof(buffer) - 1);

        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';  // Asegurar el fin de la cadena
            cout << "Datos recibidos: " << buffer << endl;
        } else {
            usleep(100000);  // Pequeña pausa para evitar sobrecarga de la CPU
        }
    }

    close(serialPort);
    return 0;
}

