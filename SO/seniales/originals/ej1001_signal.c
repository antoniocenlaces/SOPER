  /* ej1001_signal.c
   * Shell elemental con bucle de lectura de comandos con par'ametros.
   * Uso: [arre|soo] [comando][lista parametros]
   *
   * Nov 2011 Carlos J. Perez Jimenez
   * Gestion de entrada para evitar cores (cadenas vacias, etc.)
   *
   * SOLUCION ZOMBIES CON SENIALES NO SEGURAS
   *  - CAPTURA SIGCLD
   *  - PROTECCION SC BLOQUEANTE GETS
   *  - ESPERA INACTIVA
   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "error.h"

#define BUFSIZE 1024
#define TRUE 1
#define FALSE 0
#define MAL (void (*)(int))-1

int pid,espera;

void captura(int);

int main( ) {
  static char s[BUFSIZE];
  char *argt[BUFSIZE],*sp;
  int i,parate;
  void (*ff)(int);

	fprintf(stdout,"Shell pid=%d\n",getpid());

  /* Captura señal SIGCLD */
  ff=signal(SIGCLD,captura);	/* capturar senial SIGCLD */
  if (ff == MAL) syserr("signal");
	
  while(1){		
    /* SC protegida ante interrupcion de seniales */
    do {
      errno=0;	/* limpiar errno por si se interrumpe gets */
      fprintf(stderr,"\n_$ ");
      sp=gets(s);
   	} while (errno == EINTR);
//		SC sin proteger
//    fprintf(stderr,"\n_$ ");
//    sp=gets(s);

    if (sp == NULL) {printf("logout\n");exit(0);}

    argt[0]=strtok(s," ");
    if (argt[0] == NULL)  {printf("logout\n");exit(0);}
    i=strcmp(argt[0],"quit");
    if (i == 0) {printf("logout\n");exit(0);}

    i=0;
    do {
      i=i+1;
      argt[i]=strtok(NULL," \t");
    } while (argt[i] != NULL);

    i=strcmp(argt[0],"soo");
    if (i == 0) parate=TRUE;	/* indicador comando sincrono */
    else { 
      i=strcmp(argt[0],"arre");
      if (i == 0) parate=FALSE;	/* indicador comando asincrono */
      else {
        printf("\n Mande?");	/* introducir nuevo comando */
        continue;
      }
    }

    switch (pid=fork()) {
      case -1:	/* error */
        fprintf(stderr,"\nNo se puede crear proceso nuevo\n");
        syserr( "fork" );
      case 0:	/* hijo */
        if (parate == FALSE) sleep(3);	/*com. asincr. espera para que se note mas*/
        execvp(argt[1],&argt[1]);
        fprintf(stderr,"\nNo se puede ejecutar %s\n",argt[1]);
        syserr("execvp");
      default:	/* padre */
        if (parate == TRUE) {	/* comando sincrono */
          espera=1;	/* indicador de bloqueo */
          /* espera inactiva */
          while (espera != 0) pause();
          /* espera activa */
//        while (espera != 0);
        }
    }/*switch*/
  }/*while*/
}/*main*/

void captura(int n) {
  int i;
  void (*ff)(int);
  /* El programa puede bloquearse si se captura SIGCLD de hijo asincrono y
	llega SIGCLD de hijo sincrono antes de la siguiente instruccion */

  ff=signal(SIGCLD,captura);	/* capturar senial SIGCLD */
  if (ff == MAL) syserr("signal");
  i=wait(NULL);
  if (i == pid) espera=0;	/* hijo sincrono ha acabado, desbloqueo programa */
}
