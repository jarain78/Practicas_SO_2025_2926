#include <stdio.h>

int main (int argn,char *argv[])
{
    int i;
    printf("Número de argumentos: %d\n",argn);
    printf("Nombre del programa : %s\n",argv[0]);
    // Mostramos el resto de parámetros
    for(i=1;i<argn;++i) {
        printf("Argumento %d = %s\n",i,argv[i] );   
    }
    return 0;
}