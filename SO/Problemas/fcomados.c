/* fcomados.c
* Ejecución: fcomados <file>
* Programa en C que recibe el nombre de un fichero como argumento
* Ha de leer el fichero y ejecutar todos sus comandos
* Formato del fichero: cmd1 arg11 arg12 arg13 ... | cmd2 arg21 arg22 arg23 ...
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include "error.h"

#define N 10 // Número máximo de argumentos de un comando = N - 1

void ejecuta(char * arg1[], char * arg2[]);
void trocea(char * comandos[], char * arg1[], char arg2[]);

int main(int argc, char * argv[]){
    if (argc != 2) {
        printf("Uso: fcomados <file>\n");
        exit(0);
    }
    int f, i=0;
    char * arg1[N], * arg2[N];
    char * comandos[N];
    char linea[100];
    f = open(argv[1], O_RDONLY);
    if (f == -1) syserr("open");

    close(0); // FD 0 va a ser por donde se lee el fichero
    dup(f);
    close(f);
    while (gets(linea) != NULL){ // Lee del fichero mientras tenga líneas
        strtok(linea, " "); // linea contiene el primer comando de la línea
        comandos[i] = linea;
        i++;
        while (comandos[i] = strtok(NULL, " ") != NULL) {
            i++;
        } // Al salir en comandos[i] hay un NULL que indica final
          // Desde 0 a i-1 comandos contiene la línea completa incluido separador |
        trocea(comandos, arg1, arg2);
        ejecuta(arg1, arg2);
    }
}

void trocea(char * comandos[], char * arg1[], char arg2[]){
    int i = 0, j = 0, ejecutable = 1;
    while (ejecutable) {
        if (comandos[i] != "|") { // No es un separador
            arg1[i] = comandos[i];
        } else {
            ejecutable = 0;
        }
        i++;
    } // i una posición después del separador |
    arg1[(i - 1)] = NULL; // Para indicar que estamos en el final de este comando
    // i está en el segundo comando
    while (comandos[i] != NULL){
        arg2[j] = comandos[i];
        i++; j++;
    }
    arg2[j] = NULL;
}

void ejecuta(char * arg1[], char * arg2[]) {
    int * estado, pid1, pid2, fd[2], i;

    pipe(fd); // Tubería de comunicación de hijo1 (comando2) con hijo2 (comando1)
    pid1 = fork(); // Hijo1 creado
    switch (pid1)
    {
    case -1:
        syserr("fork1");
    case 0: // Código del hijo 1
        close(0); // FD 0 va a ser el canal de lectura de hijo1
        dup(fd[0]);
        close(fd[0]);
        close(fd[1]);
        // Hijo 1 va a ejecutar comando2 (segunda parte de la línea)
        sleep(1);
        execvp(arg2[0], &arg2[0]);
        syserr("execvp2");
    default:
        pid2 = fork(); // Hijo2 creado
        switch (pid2)
        {
        case -1:
            syserr("fork2");
        case 0:
            close(1); // FD 1 va a ser el canal de escritura de hijo2
            dup(fd[1]);
            close(fd[1]);
            close(fd[0]);
            sleep(1);
            execvp(arg1[0], &arg1[0]);
        default:
            i = wait(estado);
            i = wait(estado);
        } // switch2
    } // switch1
} // ejecuta