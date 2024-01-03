/*cuestion5.c
* Programa en C que obtiene el tamaño de un fichero indicado en la línea del comando
* y lo muestra por stdout, solo en 3 líneas
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char * argv[]){
      struct stat myStat;
      stat(argv[1], &myStat);
      printf("Tamaño del fichero %s: %d bytes.\n", argv[1], myStat.st_size);
}