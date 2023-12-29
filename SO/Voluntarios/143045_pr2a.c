/* 143045_pr2a.c
 * Ejercicio voluntario 2.
 * Autor: Antonio José González Almela (NIP 143045)
 * Uso: 143045_pr2a
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "error.h"

#define MAL (void (*)(int)) -1
#define N 2 //Número de procesos hijos que intercambian la señal SIGUSR1 con su padre

void capturaUSR1(int n);
void capturaUSR2(int n);
void capturaINT(int n);

int main(){
      void (*ff)(int);
      int pid[N]; // Para el padre conocer el pid de cada uno de sus hijos
      int i, myFather; // Contador y variable para almacenar el pid del padre en cada hijo

      //Captura de la señal SIGUSR1: vuelve a programar captura y escribe "-" por salida de error
      ff = signal(SIGUSR1, capturaUSR1);
      if (ff == MAL) syserr("signalUSR1");

      //Captura de la señal SIGUSR2: vuelve a programar captura. Usada para sincronizar el inicio de los hijos
      ff = signal(SIGUSR2, capturaUSR2);
      if (ff == MAL) syserr("signalUSR2");

      //Captura de la señal SIGINT: escribe "He recibido SIGINT" por salida de error y acaba el proceso
      ff = signal(SIGINT, capturaINT);
      if (ff == MAL) syserr("signalINT");

      // Bucle que lanza N procesos hijos, cada hijo se queda en pause al ser creado
      // El padre cuando los tiene todos creados los va despertando con la SIGUSR2 uno a uno
      // Cuando cada hijo despierta al recibir la SIGUSR2 entra en un bucle infinito, donde
      // primero entra en pause y espera a que el padre dirija el juego
      for (i=0; i < N; i++){
            if ((pid[i] = fork()) == 0) { // Código para los hijos
                  myFather = getppid(); // pid de mi padre para poder enviarle la SIGUSR1
// fprintf(stderr, "Soy Hijo: %d e inicio en un pause\n", (i + 1));
                  pause(); // Doy tiempo a mi padre para crear todos los hijos. Mi padre 
                           // me sacará de aquí con una SIGUSR2
// fprintf(stderr, "Soy Hijo: %d padre me ha sacado del primer pause. Voy a entrar en bucle\n", (i + 1));
                  // Inicio bucle infinito para lanzar la SIGUSR1 a mi padre
                  int j = 0;
                  while (1){
                        pause(); // Primero espero a que mi padre me diga cuando me toca enviar la SIGUSR1
//  fprintf(stderr, "Soy Hijo: %d dentro del bucle. Iteración: %d\n", (i + 1), j);
                        fprintf(stderr, "h%d", (i +1));
                        j++;
//  fprintf(stderr, "Soy Hijo: %d y me duermo 1s\n", (i + 1));
                        sleep(1); // Doy tiempo a padre para llegar a su pause
                        kill(myFather, SIGUSR1);
                  } // while
            } // if fork()
      } // for
      // Padre tiene localizados a los hijos en pid[]
      // Todo el código que sigue solo lo ejecuta el padre
      // Primero se despierta a los hijos de forma controlada
// fprintf(stderr, "\nSoy el padre con pid: %d. Hijo1 pid: %d. Hijo2 pid: %d. Y me duermo 4s\n", getpid(), pid[0] , pid[1]);
      sleep(4); // Doy tiempo a que los hijos se creen y lleguen a su pause
      for (i=0; i < N; i++){
// fprintf(stderr, "\nSoy el padre y envío SIGUSR2 a hijo %d\n", i+1);
            kill(pid[i], SIGUSR2);
      }
      // en este momento los hijos están en pause() esperando que el padre decida quien empieza el juego
      i = 0;
// fprintf(stderr, "\nSoy el padre con pid: %d. Hijo1 pid: %d. Hijo2 pid: %d\n", getpid(), pid[0] , pid[1]);
      sleep(3); // Para asegurar que los hijos han llegado a la CPU y están en su pause
      while (i < __INT_MAX__) {
// fprintf(stderr, "\nSoy el padre en iteración: %d.\n", i);
            fprintf(stderr, "p");
            kill(pid[(i % N)], SIGUSR1);
            pause();
            i++;
      }
} // main

void capturaUSR1(int n){
      void (*ff)(int);
      ff = signal(SIGUSR1, capturaUSR1);
      if (ff == MAL) syserr("signalUSR1");
      fprintf(stderr, "-");
}

void capturaUSR2(int n){
      void (*ff)(int);
      ff = signal(SIGUSR2, capturaUSR2);
      if (ff == MAL) syserr("signalUSR2");
      fprintf(stderr, "\nAlguien ha enviado una SIGUSR2\n");
}

void capturaINT(int n){
      fprintf(stderr, "\nHe recibido SIGINT\n");
      exit(0);
}