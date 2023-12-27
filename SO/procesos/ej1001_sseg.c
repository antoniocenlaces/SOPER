/* ej1001.c
 * Shell elemental con bucle de lectura de comandos con parámetros.
 * Uso: [arre|soo] [comando][lista parametros]
 * arre:  ejecucion asíncrona
 * soo:   ejecucion síncrona
 * Solución con Señales no seguras
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

int pid, espera;
void captura(int n);

int main()
{
    static char s[BUFSIZE];		/* vector de BUFSIZE caracteres */
    char *argt[BUFSIZE], *sp;	/* vector de BUFSIZE punteros */
    int i, parate;
    void (*ff)(int); // para programar captura
    sigset_t miset;
    struct sigaction miaccion;

    // Programción de la captura señal SIGCHLD
    miaccion.sa_handler = captura;
    sigemptyset(&miaccion.sa_mask);
    miaccion.sa_flags = SA_RESTART;
    i = sigaction(SIGCHLD, &miaccion, NULL);
    if (i == -1) syserr("sigprocmask");
    while(1) {
        do { // para proteger la llamada a gets
            errno = 0; // Limpia errno para detectar si gets ha sido interrumpido
            fprintf(stderr, "\n_$ ");
            sp = gets(s);
        } while (errno == EINTR);

        if (sp == NULL) {printf("logout por gets devuelve NULL\n");exit(0);}

        argt[0] = strtok(s, " ");
        if (argt[0] == NULL) {printf("logout por s contiene NULL\n");exit(0);}

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
                if (!parate) sleep(3);
                execvp(argt[1], &argt[1]);
                fprintf(stderr,"\nNo se puede ejecutar %s\n", argt[1]);
                syserr("execvp");
    
            default:    /* padre */
                if (parate) {
                    espera = 1; // La rutina de captura cuando llegue el proceso síncrono lo cambia a 0
                    while (espera !=0) pause();
                }
        } /* switch */
    } /* while */
} /* main */

void captura(int n){
    int i;
    i = wait(NULL);
    if (i == pid) espera = 0; // Hijo síncrono ha acabado
}