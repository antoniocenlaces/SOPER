/* prob10.c
* Solución al problema 10 de la colección
* Determinar el número de ficheros y su tamaño en un directorio
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "error.h"

int leelinea(int fd, char linea[BUFSIZ]);

int main(int argc, char * argv[]){
       if (argc!=2) {
        printf("Formato %s <directorio>\n", argv[0]);
        exit(1);
    }

    int fd[2], pid, longLinea, nf, i;
    int tf;
    char * campos[10];
    char linea[BUFSIZ];

    pipe(fd);
    pid = fork();

    switch (pid) {
        case -1:
            syserr("fork");
        case 0: // Código del hijo
            close(1);
            dup(fd[1]);
            close(fd[1]);
            close(fd[0]);
            execlp("ls", "ls", "-l", argv[1], 0);
            syserr("execlp");
    }
    // A partir de aquí solo ejecuta el padre
    close(0);
    dup(fd[0]);
    close(fd[0]);
    close(fd[1]);

    // Leemos primera línea de la tubería para descartar
    longLinea = leelinea(0, linea);
    if (longLinea <= 0) syserr("leelinea");

    do {
        i=0;
        longLinea = leelinea(0, linea);
        strtok(linea, " ");
        campos[i] = linea; // Primer campo de línea del ls -l
        i++;
        if (strcmp(&campos[0][0], "-") != 0){ // Si primer caracter diferente de '-' descarto la línea
            while((campos[i] = strtok(NULL, " ")) != NULL) i++;
            // campos[i] = NULL (indica que ya no hay más campos)
            nf++;
            tf = tf + atoi(campos[4]);
            printf("Fichero(%d): %s; Tamaño: %s Bytes\n", nf, campos[8], campos[4]);
        }
    } while (longLinea > 0);
    printf("\nTotal de ficheros: %d. Ocupan: %d Bytes\n", nf, tf);
}

int leelinea(int fd, char linea[BUFSIZ]){
    int p=0, eol=0, r;
    char c;
    while (!eol){
        r = read(fd, &c, 1);
        if (((int)c == 10) || r==0) {
            eol = 1;
            linea[p] = '\0';
        } else {
            linea[p] = c;
        }
        p++;
    }
    return p - 1;
}