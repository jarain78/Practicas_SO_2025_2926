#include <stdio.h>
#include <iostream>
#include <cstdlib>   // para rand() y srand()
#include <ctime>     // para time()

int main(void) {

    int minimo = 60;
    int maximo = 100;

    printf("Hola mundo\n");
    
    std::cout << "Hola mundo" << std::endl;
    std::srand(std::time(nullptr));  // Semilla aleatoria basada en el tiempo
    int r = std::rand() % 100;       // Número aleatorio entre 0 y 99
    std::cout << "Random: " << r << std::endl;

    int a = (minimo < std::rand()) < maximo;
    std::cout << "Random: " << a << std::endl;

    return 0;
}


// gcc Practica_1.c -o Practica_1