/* leelinea.c
* Función para leer una línea de un fichero abierto
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "error.h"

int leelinea(int fd, char linea[BUFSIZ]);

int main(int argc, char * argv[]){
    char linea[BUFSIZ]; int i =1;
    int longLin;
    if (argc!=2) {
        printf("Formato %s <file>\n", argv[0]);
        exit(1);
    }
    int fd = open(argv[1], O_RDONLY);
    while ((longLin=leelinea(fd, linea)) > 0) {
        printf("\nLínea: %d => '%s' (contiene %d caracteres)\n", i, linea, longLin);
        i++;
    }
    close(fd);
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