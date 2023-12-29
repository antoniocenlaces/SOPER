#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "error.h"

void captura(int);

int main(){
	int i;
	sigset_t mimask,oldmask;
	struct sigaction miaction;

	sigemptyset(&mimask);
	sigaddset(&mimask,SIGALRM);

	miaction.sa_handler=captura;
	sigemptyset(&miaction.sa_mask);
	i=sigaction(SIGALRM,&miaction,NULL);	/* capturar se�al SIGALRM */
	if (i==-1) {syserr("sigaction");exit(1);}
	printf("Se�al SIGALRM capturada\n");

	sigprocmask(SIG_BLOCK,&mimask,&oldmask);	/* bloqueo SIGALRM */
	printf("Se�al SIGALRM bloqueada\n");

/* region critica */
	alarm(3);
	printf("Alarma programada\n");
	// printf("\nSigsuspend ah devuelto: %d", i);
	// sigemptyset(&mimask);
	if (sigsuspend(&oldmask) != -1) syserr("sigsuspend");
/* fin region critica */	
	
	printf("Fin programa\n");
	
	exit(0);
}

void captura(int n){
	printf("Funcion de captura...%d\n",n);
}


