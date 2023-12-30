/* 143045_pr2b_ex.c
 * Ejercicio voluntario 2b extra.
 * Solución al ejercicio pelosig con varios procesos y con señales seguras
 * Autor: Antonio José González Almela (NIP 143045)
 * Uso: 143045_pr2b_ex
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include "error.h"


#define N 2 //Número de procesos hijos que intercambian la señal SIGUSR1 con su padre

/* N > 0 aunque no se recomienda N > 40 en Hendrix */

void capturaUSR1(int n);
void capturaUSR2(int n);
void capturaINT(int n);

int main(){
      int pid[N]; // Para el padre conocer el pid de cada uno de sus hijos
      int i, myFather; // Contador y variable para almacenar el pid del padre en cada hijo

      sigset_t myMask,oldMask;
	struct sigaction myAction1, myAction2, myAction3;

      // Programación de la captura de señales
      // Todo comportamiento ante señales programado en este proceso (padre) será heredado por sus hijos
      // SIGUSR1
      myAction1.sa_handler = capturaUSR1;
      sigemptyset(&myAction1.sa_mask);
      i = sigaction(SIGUSR1, &myAction1, NULL);
      if (i==-1) syserr("sigactionUSR1");

      // SIGUSR2
      myAction2.sa_handler = capturaUSR2;
      sigemptyset(&myAction2.sa_mask);
      i = sigaction(SIGUSR2, &myAction2, NULL);
      if (i==-1) syserr("sigactionUSR2");

      // SIGINT
      myAction3.sa_handler = capturaINT;
      sigemptyset(&myAction3.sa_mask);
      i = sigaction(SIGINT, &myAction3, NULL);
      if (i==-1) syserr("sigactionINT");

      sigemptyset(&myMask); // SET vacío
      sigemptyset(&oldMask);
      sigaddset(&myMask, SIGUSR1); // Añade SIGUSR1
      sigaddset(&myMask, SIGUSR2); // Añade SIGUSR2

      // Bloqueamos las DOS señales añadidas al SET
      sigprocmask(SIG_SETMASK, &myMask, &oldMask);

      // Bucle que lanza N procesos hijos, cada hijo se queda en pause al ser creado
      // El padre cuando los tiene todos creados los va despertando con la SIGUSR2 uno a uno
      // Cuando cada hijo despierta al recibir la SIGUSR2 entra en un bucle infinito, donde
      // primero entra en pause y espera a que el padre dirija el juego
      for (i=0; i < N; i++){
            if ((pid[i] = fork()) == 0) { // Código para los hijos
                  myFather = getppid(); // pid de mi padre para poder enviarle la SIGUSR1
                  // sigsuspend() va a restaurar el SET anterior (elimina los bloqueos de la línea 56)
                  // y después entra en pause, del que sale con la SIGUSR2 enviada por el padre
                  if (sigsuspend(&oldMask) != -1) syserr("sigsuspend");
                  // Al salir del sigsuspend() el SET myMask es restaurado => vuelven a estar bloqueadas
  
                  // Inicio bucle infinito para lanzar la SIGUSR1 a mi padre
                  while (1){
                        // Primero espero a que mi padre me diga cuando me toca enviar la SIGUSR1
                        if (sigsuspend(&oldMask) != -1) syserr("sigsuspend");
                        fprintf(stderr, "h%d", (i + 1));
                        kill(myFather, SIGUSR1);
                  } // while
            } // if fork()
      } // for
      
      // Padre tiene localizados a los hijos en pid[]
      // Todo el código que sigue solo lo ejecuta el padre
      // sleep(1);
      // Bucle para despertar a los hijos. Inmediatamente entran en un sigsuspend()
      for (i=0; i < N; i++){
            kill(pid[i], SIGUSR2);
      }
      
      // en este momento los hijos están en pause() esperando que el padre decida quien empieza el juego
      i = 0;
      sleep(1);
      while (i < 2147483647) {
            fprintf(stderr, "p");
            kill(pid[(i % N)], SIGUSR1);
            if (sigsuspend(&oldMask) != -1) syserr("sigsuspend");
            i++;
      }
      // No será fácil que llegue a ejecutarse el código siguiente, pero si alguien tiene tanta paciencia
      // Hay que acabar controladamente a todos los hijos
      for (i=0; i < N; i++){
            kill(pid[i], SIGINT); // Usamos la SIGINT capturada por todos los hijos para que acaben controladamente
      }
      // Ahora solo falta hacer un wait para cada hijo y acabar
      // Aunque el padre no hiciese wait por los hijos, estos desaparecen al acabar el padre
      for (i=0; i < N; i++){
            pid[0] = wait(NULL);
      }
      exit(0);
} // main

void capturaUSR1(int n){
      fprintf(stderr, "-");
}

void capturaUSR2(int n){
      fprintf(stderr, "\nAlguien ha enviado una SIGUSR2\n");
}

void capturaINT(int n){
      fprintf(stderr, "\nHe recibido SIGINT\n");
      exit(0);
}
