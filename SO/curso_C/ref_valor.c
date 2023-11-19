#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void pasoValorReferencia(int *array, int valor) {
    array[5]= -8.6;
    valor =4;
}

int main(int argc, char *argv[]) {
   int array[10]={0,0,0,0,0,0,0,0,0,0};
   pasoValorReferencia(array,array[3]);
   printf("Array[5] vale: %d y array[3] vale:%d\n",array[5],array[3]);
   return 0;
}