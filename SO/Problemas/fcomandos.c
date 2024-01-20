/* fcomados.c
* Ejecución: fcomados <file>
* Programa en C que recibe el nombre de un fichero como argumento
* Ha de leer el fichero y ejecutar todos sus comandos
* Formato del fichero: cmd1 arg11 arg12 arg13 ... | cmd2 arg21 arg22 arg23 ...
* Ejercicio extra de los videos
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
void trocea(char * comandos[], char * arg1[], char * arg2[]);

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
    int ciclo = 0;
    while (gets(linea) != NULL){ // Lee del fichero mientras tenga líneas
    printf("Inicia ciclo: %d\n",ciclo);
    printf("%s\n", linea);
        strtok(linea, " "); // linea contiene el primer comando de la línea
        comandos[i] = linea;
    printf("Obtenido primer comando: %s\n", comandos[i]);
        i++;
        while ((comandos[i] = strtok(NULL, " ")) != NULL) {
            printf("Otro comando(%d): %s\n", i,comandos[i]);
            i++;
        } // Al salir en comandos[i] hay un NULL que indica final
          // Desde 0 a i-1 comandos contiene la línea completa incluido separador |
        trocea(comandos, arg1, arg2);
        ejecuta(arg1, arg2);
        i=0;
    }
}

void trocea(char * comandos[], char * arg1[], char * arg2[]){
    int i = 0, j = 0, ejecutable = 1;

  i=0;
           printf("En trocea mostrar el contenido de comandos en lista vertical.\n");
         while (comandos[i] != NULL) {
             printf("%s\n", comandos[i]);
             i++;
        }
        i=0;

    while (ejecutable) {
        if ((strcmp(comandos[i], "|")) != 0) { // No es un separador
            arg1[i] = comandos[i];
             printf("Estoy sacando elemento %d que es %s\n", i, arg1[i]);
        } else {
             printf("Encontrado un separador\n");
            ejecutable = 0;
        }
        i++;
    } // i una posición después del separador |
    arg1[(i - 1)] = NULL; // Para indicar que estamos en el final de este comando
    // i está en el segundo comando
    printf("Inicia troceo del segundo comando\n");
    while (comandos[i] != NULL){
        arg2[j] = comandos[i];
        printf("Estoy sacando elemento %d que es %s\n", j, arg2[j]);
        i++; j++;
    }
    arg2[j] = NULL;
}

void ejecuta(char * arg1[], char * arg2[]) {
    int * estado, pid1, pid2, fd[2], i;
    estado=NULL;
fprintf(stderr,"Iniciado ejecuta para el comando: %s | %s\n",arg1[0], arg2[0]);
    pipe(fd); // Tubería de comunicación de hijo1 (comando2) con hijo2 (comando1)
    char m[33];
    pid1 = fork(); // Hijo1 creado
    fprintf(stderr,"Mi Hijo1 es pid: %d\n",pid1);
    switch (pid1)
    {
    case -1:
        syserr("fork1");
    case 0: // Código del hijo 1
        close(0); // FD 0 va a ser el canal de lectura de hijo1
        dup(fd[0]);
        close(fd[0]);
        close(fd[1]);
        read(0, m, 33);
        printf("%s", m);
        fprintf(stderr,"Hijo1 debería ejecutar: %s\n", arg2[0]);
        // Hijo 1 va a ejecutar comando2 (segunda parte de la línea)
        execvp(arg2[0], &arg2[0]);
        syserr("execvp2");
    default:
        pid2 = fork(); // Hijo2 creado
        fprintf(stderr, "Mi Hijo2 es pid: %d\n",pid2);
        switch (pid2)
        {
        case -1:
            syserr("fork2");
        case 0:
            close(1); // FD 1 va a ser el canal de escritura de hijo2
            dup(fd[1]);
            close(fd[1]);
            close(fd[0]);
            write(1,"Mensaje inicial de Hijo2 a Hijo1\n",33);
            fprintf(stderr, "Hijo2 debería ejecutar: %s\n", arg1[0]);
            execvp(arg1[0], &arg1[0]);
            syserr("execvp1");
        default:
        fprintf(stderr, "Padre ha creado dos hijos y va a cerrar pipe y esperar por hijos\n");
        sleep(2);
            // close(fd[0]);
            // close(fd[1]);
            i = wait(estado);
            fprintf(stderr,"Mi hijo %d ha terminado correctamente con estado: %d\n",i, *estado);
            i = wait(estado);
            fprintf(stderr,"Mi hijo %d ha terminado correctamente con estado: %d\n",i, *estado);
        } // switch2
    } // switch1
} // ejecuta