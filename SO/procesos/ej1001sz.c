/* ej1001.c
 * Shell elemental con bucle de lectura de comandos con parámetros.
 * Uso: [arre|soo] [comando][lista parametros]
 * arre:  ejecucion asíncrona
 * soo:   ejecucion síncrona
 * Modificado para que un segundo hijo ejecute el execvp en caso de ejecución asíncrona
 * para ejecución síncrona el primer hijo hace execvp
 * Evita la creación de zombies de la ejecución asíncrona
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "error.h"

#define BUFSIZE 1024
#define TRUE     1
#define FALSE    0

int main()
{
    char s[BUFSIZE];		/* vector de BUFSIZE caracteres */
    char *argt[BUFSIZE];	/* vector de BUFSIZE punteros */
    int i, parate, pid;

    while(1) {
        fprintf(stderr, "\n_$ ");
        gets(s);
        // fgets(s, sizeof(s), stdin); s[strlen(s)-1] = 0;

        if (strlen(s) == 0) continue;
        argt[0] = strtok(s, " ");

        if (0 == strcmp(argt[0], "quit")) {
            fprintf(stderr, "Me han pedido logout\n");
            exit(0);
        }

        for (i = 1; 
             (argt[i] = strtok(NULL," \t")) != NULL;
             i++ );

        if (0 == strcmp(argt[0], "soo"))
            parate = TRUE;
        else {
            if (0 == strcmp(argt[0], "arre"))
                parate = FALSE;
            else {
                printf("\n Mande?");
                continue;
            }
        }
        switch(pid = fork()) {
            case -1:    /* error */
                fprintf(stderr, "\nNo se puede crear proceso nuevo\n");
                syserr("fork");
      
            case 0:    /* hijo */
                if (!parate) { // Solo si es comportamiento asíncrono se va a crear un nieto
                  switch (pid = fork()){ // Nieto creado
                        case -1:    /* error */
                              fprintf(stderr, "\nNo se puede crear proceso nuevo\n");
                              syserr("fork");
                        case 0: // El nieto se bloquea 3s para dar sensación de asíncrono
                              sleep(3);
                              break;
                        default:
                        exit(0); // Hacemos que el primer hijo muera aquí. Solo queda el padre y el nieto.
                  } /*switch interno*/
                } // if (!parate)
                execvp(argt[1], &argt[1]); // Para síncrono lo ejecuta el hijo para asíncrono lo ejecuta el nieto
                                           // Cuando este nieto acaba será adoptado por el init
                fprintf(stderr,"\nNo se puede ejecutar %s\n", argt[1]);
                syserr("execvp");
            default:    /* padre, que ahora siempre hace wait, tanto para síncrono como asíncrono*/
               i = wait(NULL);
               if (i == -1) { syserr("wait");exit(1);}
        } /* switch */
    } /* while */
} /* main */
