/*  Ene 2013 Carlos J. Perez Jimenez
 *  Ejemplo de uso de threads
 *  con parametros y resultados
 *  Programa de 2 threads para comparar tiempos
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "error.h"

void *start(void *);

/* var global compartidas por todos los threads */
int n,suma;

struct arg {	/* estructura para pasar mas de un parametro al thread */
	int ini;	/* indice inicial*/
	int fin;	/* indice final */
};

void main(int argc,char *argv[]) {
	int i,suma,suma2,error;
	pthread_t tid;
	struct arg param;

	if (argc != 2){printf("Uso: thr_2 <int>\n");exit(1);}
	n=atoi(argv[1]);

	printf("Calculando S(%d) en 2 threads\n",n);
	
	tid=pthread_self();
	printf("\tSoy el thread %d (1,%d)\n",(int)tid,n/2);

	/* creando thread para suma2 */
	param.ini=n/2+1;
	param.fin=n;
	error=pthread_create(&tid,NULL,start,&param);
	if (error!=0) syserr(pthread_create);

	/* suma1 */
	suma=0;
	for (i=1;i<=n/2;i=i+1) suma=suma+1;
	printf("\tSuma1 %d\n",suma);

	pthread_join(tid,(void **)&suma2);
	suma=suma+suma2;
	printf("Suma=%d\n",suma);
}

void *start(void *p) {
	pthread_t tid;
	int i,ini,fin,tmp;

	tid=pthread_self();
	ini=((struct arg *)p)->ini;
	fin=((struct arg *)p)->fin;
	printf("\tSoy el thread %d (%d,%d)\n",(int)tid,ini,fin);
	tmp=0;
	for (i=ini;i<=fin;i=i+1) tmp=tmp+1;
	printf("\tSuma2 %d\n",tmp);

	pthread_exit((void *)tmp);
}
