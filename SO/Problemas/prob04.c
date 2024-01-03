/* prob04.c
* Solución al problema 4 de la colección
* Matar a todos los procesos del usuario excepto el shell

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "error.h"

int main(int argc, char* argv[], char* envp[]){
    int i = 0, fin = 1;
    while(fin && envp[i] != NULL){
        // printf("Variable de entorno %d: %s\n", i, envp[i]);
        strtok(envp[i], "="); // Ahora envp[i] está troceado hasta donde antes estaba =
        if (strcmp(envp[i], "USER") == 0) {
            fin = 0;
            envp[i] = strtok(NULL, "\n");
            i--;
        }
        i++;
    }
    if (fin) {
        printf("No encuentro la variable de entorno USER\n");
        exit(1);
    }
    printf("La variable USER vale: %s\n", envp[i]);

    // envp[i] apunta a la cadena con el nombre de usuario que ha ejecutado prob04

    int fd[2], pid, yo, ppid;
    char cadena[100];

    ppid = getppid();
    yo = getpid();

    pipe(fd); // Tubería de comunicación. Hijo escribe y padre lee.
    pid = fork();
    switch (pid){
        case -1:
            syserr("fork");
        case 0: // El hijo se queda con extremo de escritura abierto en FD 1
            close(fd[0]);
            close(1);
            dup(fd[1]);
            close(fd[1]);
            execlp("ps", "ps", "-u", envp[i], 0);
            syserr("execlp");
        default: // El padre se queda con el extremo de lectura abierto en FD 0
            close(fd[1]);
            close(0);
            dup(fd[0]);
            close(fd[0]);
    }
    gets(cadena); // Primera línea de ps es descartada
    printf("%s\n", cadena);

    int proceso;
    while (gets(cadena) != NULL){
        printf("%s\n", cadena);
        strtok(cadena, " \t");
        proceso = atoi(cadena);
        printf("Siguiente pid leido: %d\n", proceso);
        if (proceso != yo && proceso != pid && proceso != ppid){
            kill(proceso, SIGKILL);
        }
    }

}