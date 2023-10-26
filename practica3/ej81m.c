/*	ej81m.c	*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "error.h"

int main()
{ 
    int pid;
    int id = fork();
    switch(id) {
        case -1:
    	    syserr("fork");
        case 0: // Código que ejecuta el hijo
            printf("Soy el hijo\n");
            pid = getppid();
            printf("\tpid padre antes = %d\n",pid);
            sleep(25); 
            pid = getppid();
            printf("\tpid padre despues = %d\n",pid);
            exit(15);
        default: // Código del padre
        printf("Soy el padre\n");
        printf("Mi PID es: %d\n", getpid());
        printf("El pid del hijo = %d\n",id);
            sleep(15);
            exit(0);
    } 
}
