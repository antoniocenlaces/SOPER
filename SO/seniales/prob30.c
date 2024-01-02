/* prob30.c
* Solución al problema 30 de la colección
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "error.h"

void capturaINT(int n);
void capturaQUIT(int n);
void capturaALRM(int n);

int fin;

int main(){
      printf("Inicio programa cronómetro. pid=%d\n", getpid());

      signal(SIGINT, capturaINT);
      signal(SIGQUIT, capturaQUIT);
      signal(SIGALRM, capturaALRM);

      alarm(60);
      fin = 0;
      while (fin == 0) pause();
      exit(0);
}

void capturaQUIT(int n){
      printf("Finalizo por SIGQUIT 3 (Ctrl-\\)\n");
      fin = 1;
}

void capturaALRM(int n){
      printf("Finalizo por SIGALRM (60s.)\n");
      fin = 1;
}

void capturaINT(int n){
      static int crono = 0;
      static struct timeval reloj;
      struct timeval t2;

      signal(SIGINT, capturaINT);

      if (crono == 0){ // Crono a a iniciar
            gettimeofday(&reloj, NULL);
            printf("Crono iniciado\n");
      } else { // Crono ya está iniciado
            gettimeofday(&t2, NULL);
            t2.tv_sec = t2.tv_sec - reloj.tv_sec;
            t2.tv_usec = t2.tv_usec - reloj.tv_usec;
            if (t2.tv_usec < 0){
                  t2.tv_usec = t2.tv_usec + 1000000;
                  t2.tv_sec--;
            }
            printf("Han transcurrido %d.%06d segundos.\n", t2.tv_sec, t2.tv_usec);
      }
      crono = 1 - crono;
}