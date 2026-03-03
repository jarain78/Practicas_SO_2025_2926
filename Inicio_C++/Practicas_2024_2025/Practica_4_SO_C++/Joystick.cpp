#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        std::cerr << "Error inicializando SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (SDL_NumJoysticks() < 1) {
        std::cout << "No hay joysticks conectados." << std::endl;
    } else {
        SDL_Joystick* joystick = SDL_JoystickOpen(0);
        if (joystick == nullptr) {
            std::cerr << "No se pudo abrir el joystick: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }

        std::cout << "Joystick abierto: " << SDL_JoystickName(joystick) << std::endl;
        std::cout << "Ejes disponibles: " << SDL_JoystickNumAxes(joystick) << std::endl;
        std::cout << "Botones disponibles: " << SDL_JoystickNumButtons(joystick) << std::endl;

        SDL_Event e;
        bool quit = false;

        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_JOYAXISMOTION) {
                    std::cout << "Eje " << (int)e.jaxis.axis << " valor: " << e.jaxis.value << std::endl;
                } else if (e.type == SDL_JOYBUTTONDOWN) {
                    std::cout << "Botón " << (int)e.jbutton.button << " presionado." << std::endl;
                } else if (e.type == SDL_JOYBUTTONUP) {
                    std::cout << "Botón " << (int)e.jbutton.button << " liberado." << std::endl;
                }
            }
            SDL_Delay(10); // Pequeño delay para no consumir 100% CPU
        }

        SDL_JoystickClose(joystick);
    }

    SDL_Quit();
    return 0;
}


/*g++ -Wall -Wextra -g3 -O0 \
  Practica_4_SO_C++/Joystick.cpp \
  -o build/Debug/outDebug*/

//g++ Practica_4_SO_C++/Joystick.cpp -o joystick -lSDL2//

// g++ Practica_4_SO_C++/Joystick.cpp -o joystick -I /opt/homebrew/include/SDL2 -L /opt/homebrew/lib -lSDL2
// g++ Practica_4_SO_C++/Joystick.cpp -o joystick $(sdl2-config --cflags --libs)

// g++ Practica_4_SO_C++/Joystick.cpp -o joystick -I/opt/homebrew/include/SDL2 -L/opt/homebrew/lib -lSDL2
