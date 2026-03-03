#include "RobotControl.h"

int main() {
    initializeSockets();

    // Obtener posición actual del servidor
    getCurrentPosition();

    // Enviar una posición como obstáculo
    float x = 10.5f, y = 0.75f, z = 20.0f;
    sendPosition(x, y, z);

    // Enviar una trayectoria
    sendPath();

    cleanupSockets();
    return 0;
}

/*clang++ -Wall -Wextra -g3 -O0 \
  Practica_3_2025/main.cpp \
  Practica_3_2025/RobotControl.cpp \
  -o build/Debug/outDebug
*/

/*g++ -Wall -Wextra -g3 -O0 \
  Practica_3_2025/main.cpp \
  Practica_3_2025/RobotControl.cpp \
  -o build/Debug/outDebug*/

  // ./build/Debug/outDebug