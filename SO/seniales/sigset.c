/*  Nov 2011 Carlos J. Perez Jimenez
 *  Gestion de conjuntos de señales (sigset), funciones
 *  sigfillset
 *  sigemptyset
 *  sigaddset
 *  sigdelset
 *  sigismember
*/

#include <stdio.h>
#include <signal.h>

#define TRUE 1
#define FALSE 0

void main() {
	int i,j;
	sigset_t miset;
	
	/* GESTION DE CONJUNTOS DE SEÑALES */

	printf("Lleno mi conjunto de signals\n");
	sigfillset(&miset);	/* sigset es de 48 bits en solaris */
	i=sigismember(&miset,SIGALRM);
	if (i==TRUE) printf("\tSIGALRM esta en sigset\n");
	else printf("\tSIGALRM no esta en sigset\n");
	i=sigismember(&miset,SIGKILL);
	if (i==TRUE) printf("\tSIGKILL esta en sigset\n");
	else printf("\tSIGKILL no esta en sigset\n");

	printf("Vacio mi conjunto de signals\n");
	sigemptyset(&miset);
	i=sigismember(&miset,SIGCLD);
	if (i==TRUE) printf("\tSIGCLD esta en sigset\n");
	else printf("\tSIGCLD no esta en sigset\n");
	i=sigismember(&miset,SIGTERM);
	if (i==TRUE) printf("\tSIGTERM esta en sigset\n");
	else printf("\tSIGTERM no esta en sigset\n");


	printf("Añado SIGCLD a mi conjunto de signals\n");
	sigaddset(&miset,SIGCLD);
	i=sigismember(&miset,SIGCLD);
	if (i==TRUE) printf("\tSIGCLD esta en sigset\n");
	else printf("\tSIGCLD no esta en sigset\n");

	printf("Quito SIGCLD de mi conjunto de signals\n");
	sigdelset(&miset,SIGCLD);
	i=sigismember(&miset,SIGCLD);
	if (i==TRUE) printf("\tSIGCLD esta en sigset\n");
	else printf("\tSIGCLD no esta en sigset\n");
}
