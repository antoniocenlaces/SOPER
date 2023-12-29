/* pelosig.c
 * Ejemplo 2 para mostrar problemas con señales no seguras.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "error.h"

void f(int);

int main(){
      int pid;
      signal(SIGUSR1, f);
      if ((pid = fork() == 0)) { // Código del hijo
            pid = getppid();
            while (1){
                  fprintf(stderr, "h");
                  kill(pid, SIGUSR1);
                  pause();
            }
      } else {
            pause(); // Padre entra en pause después de crear al hijo
            while (1){ // El hijo lo saca del pause y entra en el bucle de intercambio con el hijo
                  fprintf(stderr, "p");
                  kill(pid, SIGUSR1);
                  pause();
                  // sleep(1);
            }
      }
}

void f(int) {
      signal(SIGUSR1, f);
      fprintf(stderr, "-");
}