  /* ej1001_signal.c
   * Shell elemental con bucle de lectura de comandos con par'ametros.
   * Uso: [arre|soo] [comando][lista parametros]
   *
   * Nov 2011 Carlos J. Perez Jimenez
   * Gestion de entrada para evitar cores (cadenas vacias, etc.)
   *
   * SOLUCION ZOMBIES CON SENIALES SEGURAS
   *  - CAPTURA SIGCLD CON REINICIO DE SCs INTERRUMPIDAS
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

int pid,espera;

void captura(int);

int main( ) {
  static char s[BUFSIZE];
  char *argt[BUFSIZE],*sp;
  int i,parate;
  struct sigaction miaction;

	fprintf(stdout,"Shell pid=%d\n",getpid());

  /* Captura senial SIGCLD */
  miaction.sa_handler=captura;	/* funcion gestion SIGCLD */
	sigemptyset(&miaction.sa_mask);
  miaction.sa_flags=SA_RESTART;	/* reinicio auto de SCs interrumpidas */
//	fprintf(stderr,"Tam flags %d %x\n",sizeof(miaction.sa_flags),miaction.sa_flags);
//	exit(1);
  i=sigaction(SIGCLD,&miaction,NULL);	/* capturo SIGCLD segura */
  if (i==-1) {syserr("sigaction");exit(1);}	
	
  while(1){		
    fprintf(stderr,"\n_$ ");
    sp=gets(s);
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
          while (espera != 0) pause();
        }
    }/*switch*/
  }/*while*/
}/*main*/

void captura(int n) {
  int i;

  i=wait(NULL);
//	fprintf(stdout,"Senial %d\n",n);
	if (i == pid) espera=0;	/* hijo sincrono ha acabado, desbloqueo programa */
}
