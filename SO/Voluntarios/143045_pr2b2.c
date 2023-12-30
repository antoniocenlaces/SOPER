/* 143045_pr2b2.c
 * Ejercicio voluntario 2b.
 * Solución al ejercicio pelosig con señales seguras
 * Autor: Antonio José González Almela (NIP 143045)
 * Uso: 143045_pr2b2
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "error.h"

void captura(int n);

int main(){
      int i, pid;
      sigset_t myMask,oldMask;
	struct sigaction myAction;

       // Programación de la captura de señales
      // Todo comportamiento ante señales programado en este proceso (padre) será heredado por sus hijos
      // SIGUSR1
      myAction.sa_handler = captura;
      sigemptyset(&myAction.sa_mask);
      i = sigaction(SIGUSR1, &myAction, NULL);
      if (i==-1) syserr("sigaction");

      sigemptyset(&myMask); // SET vacío
      sigemptyset(&oldMask);
      sigaddset(&myMask, SIGUSR1); // Añade SIGUSR1

      // Bloqueamos la señale añadida al SET
      sigprocmask(SIG_BLOCK, &myMask, &oldMask);

      if ((pid = fork() == 0)) { // Código del hijo
            pid = getppid();
            while (1){
                  fprintf(stderr, "h");
                  kill(pid, SIGUSR1);
                  if (sigsuspend(&oldMask) != -1) syserr("sigsuspend");
            }
      } else {
            if (sigsuspend(&oldMask) != -1) syserr("sigsuspend"); // Padre entra en pause después de crear al hijo
            while (1){ // El hijo lo saca del pause y entra en el bucle de intercambio con el hijo
                  fprintf(stderr, "p");
                  kill(pid, SIGUSR1);
                  if (sigsuspend(&oldMask) != -1) syserr("sigsuspend");
                  // sleep(1);
            }
      }
}

void captura(int n){
      fprintf(stderr, "-");
}