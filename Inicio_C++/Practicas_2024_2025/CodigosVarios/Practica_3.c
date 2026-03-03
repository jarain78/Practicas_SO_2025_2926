#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argn, char *argv[]) {

    if ( fork() == -1 ) {
        printf ("La llamada a fork ha fallado ...\n");
        exit (1); // o return 1;
    }
    else {
        printf ("La llamada a fork se ha realizado con exito...\n");
        exit (0); // o return 0;
    }
}

// https://www.freecodecamp.org/news/how-to-write-and-run-c-cpp-code-on-visual-studio-code/