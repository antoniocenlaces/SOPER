/*  Ene 2012 Carlos J. Perez Jimenez
 *  Ejemplo de uso de threads (ejemplo clase)
 *  sin parametros ni resultados
 *	acceso inteligente en exclusion mutua a suma
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "error.h"

void *start(void *);

int n,suma;	/* var global compartidas por todos los threads */

void main(int argc,char *argv[]) {
	int i,par,error;
	pthread_t tid;

	if (argc != 2) {printf("Uso: threads <int>\n");exit(1);};
	n=atoi(argv[1]);

	printf("Calculando S(%d) en 2 threads\n",n);
	tid=pthread_self();
	printf("\tSoy el thread %d\n",(int)tid);

	/* creando thread para suma de impares */
	error=pthread_create(&tid,NULL,start,NULL);	
	if (error!=0) syserr(pthread_create);

	/* calculando suma de pares */
	par=0;
	for (i=2;i<=n;i=i+2) par=par+i;
	printf("\tSuma pares %d\n",par);

	pthread_join(tid,NULL);	/* espera terminacion thread*/
	suma=suma+par;	/* asegura acceso exclusivo a suma */
	printf("Suma=%d\n",suma);
}

void *start(void *arg) {	/* funcion de comienzo ejec. thread */
	pthread_t tid;
	int tmp,i;

	tid=pthread_self();
	printf("\tSoy el thread %d\n",(int)tid);
	tmp=0;
	for (i=1;i<=n;i=i+2) tmp=tmp+i;
	printf("\tSuma impares %d\n",tmp);
	suma=suma+tmp;
}
