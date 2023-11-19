#include <stdio.h>

int main(int argc, char *argv[], char *env[]) {
   int i;
   printf("El número de argumentos recibido: %d\n",argc);
   for (i=0; i<argc; i++) {
    printf("Parámetro %d = %s\n",i, argv[i]);
   }
   for (i=0; env[i]!=NULL; i++) {
    printf("La variable de entorno %d vale: %s\n",i, env[i]);
   }
   return 0;
}