/*ej73.c
Ejemplo de teoría Sistemas Operativos
uso de fork() para crear un shell muy simple*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "error.h"

#define BUFFSIZE 1024

int main() {
      static char s[BUFFSIZE];
      char * argt[BUFFSIZE];
      int i;
      for (;;) {/*Infinito*/
            fprintf(stderr, "\n_$ ");
            if ( gets(s) == NULL) {
                  printf("logout por entrada NULL\n");
                  exit(0);
            }
            argt[0] = strtok( s, " "); /*argt[0] contiene el comando a ejecutar*/
            if ( 0 == strcmp( argt[0], "quit")) {
                  printf("logout por entrada quit\n");
                  exit(0);
            }
            for ( i =1; (argt[i] = strtok(NULL, " \t")) != NULL; i++);
            switch (fork())
            {
            case -1:
                  fprintf(stderr, "\nNo se puede crear proceso nuevo\n");
                  syserr("fork");
                  break;
            case 0:
                  sleep(2);
                  execvp( argt[0], &argt[0]);
                  fprintf(stderr, "\nNo se puede ejecutar el exec()\n");
                  syserr("execvp");
                  break;
            default:
                  if ( wait(NULL) == -1 ) syserr("wait");
                  break;
            }
      }

}