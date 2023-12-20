/*ej701.c
Ejemplo de teoría Sistemas Operativos
uso de fork() para crear un hijo*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "error.h"

int main(int argc, char * argv[]) {
  int pidh, err;
  printf("Inicio prueba\n");
  pidh = fork();
  if ( pidh == 0 ) { /* C�digo del hijo */
    printf("\nSoy el hijo con pid: %d\n", getpid());
    printf("\nDespu�s del fork me ha devuelto: %d\n", pidh);
    exit(0);
  }
  /* C�digo del padre */
  fprintf(stderr, "Padre: Antes de sleep\n");
  sleep(1);
  err = wait(NULL);
  if ( err == -1 ) syserr("wait");
  
  printf("\nSoy el Padre y tengo pid: %d\n", getpid());
  printf("\nEl fork me ha devuelto: %d\n", pidh);
}