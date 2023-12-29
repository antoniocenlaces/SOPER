/* 143045_pr2a.c
 * Ejercicio voluntario 2a.
 * Autor: Antonio José González Almela (NIP 143045)
 * Uso: 143045_pr2a
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include "error.h"

#define MAL (void (*)(int)) -1
#define N 2 //Número de procesos hijos que intercambian la señal SIGUSR1 con su padre

/* N > 0 aunque no se recomienda N > 40 en Hendrix */

void capturaUSR1(int n);
void capturaUSR2(int n);
void capturaINT(int n);

int main(){
      void (*ff)(int);
      int pid[N]; // Para el padre conocer el pid de cada uno de sus hijos
      int i, myFather; // Contador y variable para almacenar el pid del padre en cada hijo

      // Todo comportamiento ante señales programado en este proceso (padre) será heredado por sus hijos
      //Captura de la señal SIGUSR1: vuelve a programar captura y escribe "-" por salida de error
      ff = signal(SIGUSR1, capturaUSR1);
      if (ff == MAL) syserr("signalUSR1");

      //Captura de la señal SIGUSR2: vuelve a programar captura, muestra mensaje. Usada para sincronizar el inicio de los hijos
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
                  pause(); // Doy tiempo a mi padre para crear todos los hijos. Mi padre 
                           // me sacará de aquí con una SIGUSR2
                  // Inicio bucle infinito para lanzar la SIGUSR1 a mi padre
                  while (1){
                        pause(); // Primero espero a que mi padre me diga cuando me toca enviar la SIGUSR1
                        fprintf(stderr, "h%d", (i + 1));
                        // sleep(1); // Doy tiempo a padre para llegar a su pause
                                     // En máquinas muy rápidas es necesario introducir una pausa antes
                                     // del hijo enviar la señal al padre
                        kill(myFather, SIGUSR1);
                  } // while
            } // if fork()
      } // for
      
      // Padre tiene localizados a los hijos en pid[]
      // Todo el código que sigue solo lo ejecuta el padre
      
      // Esto es una estimación del retardo necesario para garantizar que los procesos llegan al pause
      // antes de ser señalados
      int retardo;
      if (N > 5) retardo = 5;
              else retardo = N;
      
      // Primero se despierta a los hijos de forma controlada
      sleep(retardo); // Doy tiempo a que los hijos se creen y lleguen a su pause
      for (i=0; i < N; i++){
            kill(pid[i], SIGUSR2);
      }
      
      // en este momento los hijos están en pause() esperando que el padre decida quien empieza el juego
      i = 0;
      sleep(retardo); // Para asegurar que los hijos han llegado a la CPU y están en su pause
      while (i < 2147483647) {
            fprintf(stderr, "p");
            kill(pid[(i % N)], SIGUSR1);
            pause();
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
