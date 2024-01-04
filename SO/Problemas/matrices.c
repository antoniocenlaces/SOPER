/* matrices.c
* Pruebas de matrices de punteros a char en C
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "error.h"

void escribe(char * c){
    printf(" %s\n", c);
}

int main(){
    char * c[10][10];
    c[0][0]="ls";
    c[0][1]="-lr";
    c[0][2]=NULL;

    int i = 0;
    while (c[0][i] != NULL){
        printf("Este es el contenido de c[0][%d]:", i);
        escribe(c[0][i]);
        i++;
    }
    char * s[10];
    s[0]="ls";
    s[1]="-la";
    s[2]=NULL;

    char * m;
    m = *(s + 1);
    escribe(m);

    m=*(c[0]);
    escribe(m);
     m=*(c[0] + 1);
    escribe(m);
}