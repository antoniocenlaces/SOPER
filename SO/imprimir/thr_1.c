/*  Ene 2013 Carlos J. Perez Jimenez
 *  Ejemplo de uso de threads
 *  Programa de 1 thread para comparar tiempos
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "error.h"

/* var global compartidas por todos los threads */
int n,suma;

void main(int argc,char *argv[]) {
	int i,error;
	pthread_t tid;

	if (argc != 2){printf("Uso: thr_1 <int>\n");exit(1);}
	n=atoi(argv[1]);

	printf("Calculando S(%d) en 1 thread\n",n);
	
	tid=pthread_self();
	printf("\tSoy el thread %d\n",(int)tid);

	/* suma */
	suma=0;
	for (i=1;i<=n;i=i+1) suma=suma+1;
	printf("Suma=%d\n",suma);
}
