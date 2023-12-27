/*  Nov 2011 Carlos J. Perez Jimenez
 *  Bloqueo de se�ales seguras
 *  sigprocmask
 *  sigpending
 *  sigaction
 *  struct sigaction
 *  
 *  Desbloqueo inseguro de se�ales seguras y motivacion de sigsuspend
*/

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "error.h"

#define TRUE 1
#define FALSE 0

void captura(int);

void main() {
	int i,j;
	sigset_t mimask,oldmask;
	struct sigaction miaction,oldaction;
	
	/* GESTION DE SE�ALES BLOQUEADAS */
	
	/* PRIMERO leo se�ales bloqueadas por defecto (ninguna) */
	i=sigprocmask(0,NULL,&mimask);
	if (i==-1) syserr("sigprocmask");
	for (i=1;i<NSIG;i=i+1) {	/* se�ales 1 a 48 en Solaris 10*/
		j=sigismember(&mimask,i);
		if (j==TRUE) printf("Bloqueada se�al %d\n",i);
		else if (j<0) {printf("Fallo %d\n",i);exit(1);}
	}

	/* SEGUNDO creo mi propio conjunto de se�ales bloqueadas */
	
	sigemptyset(&mimask);
	sigaddset(&mimask,SIGALRM);
	i=sigprocmask(SIG_BLOCK,&mimask,&oldmask);
	if (i==-1) syserr("sigprocmask");
	printf("La se�al SIGALRM esta bloqueada\n");

	alarm(4);	/* programo alarma en 4 seg */
	printf("La se�al SIGALRM esta programada para dentro de 4 segundos\n");

	do {
		sleep(1);	//para eliminar espera activa
		i=sigpending(&mimask);
		if (i==-1) syserr("sigpending");
		i=sigismember(&mimask,SIGALRM);
		if (i<0) syserr("sigismember");
	} while (i==FALSE);
	
	printf("La se�al SIGALRM esta pendiente de gestionarse\n");
	
	/* NECESIDAD DE SIGSUSPEND, DESBLOQUEO INSEGURO DE SIGALRM */
	
	/* capturo SIGALRM de forma segura con sigaction */
	miaction.sa_handler=captura;
	sigemptyset(&miaction.sa_mask);
	i=sigaction(SIGALRM,&miaction,&oldaction);
	if (i<0) syserr("sigaction");
	
	/* desbloqueo de forma insegura SIGALRM */
	printf("La se�al SIGALRM va a ser desbloqueada de forma insegura\n");
	i=sigprocmask(SIG_SETMASK,&oldmask,NULL);
	if (i==-1) syserr("sigprocmask");
	printf("La se�al SIGALRM desbloqueada de forma insegura\n");
	/* EL PROCESO QUEDA BLOQUEADO EN EL PAUSE */ 
	pause();
	
}

void captura(int n) {
	printf("Estoy en la rutina de la se�al %d\n",n);
}
