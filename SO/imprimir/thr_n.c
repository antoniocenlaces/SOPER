/*  Ene 2013 Carlos J. Perez Jimenez
 *  Ejemplo de uso de threads
 *  con parametros y resultados
 *  Programa de <n> threads para comparar tiempos
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "error.h"

void *start(void *);

/* var global compartidas por todos los threads */
int n,n_thr;

struct arg {	/* estructura para pasar mas de un argumento al thread */
	int ini;	/* indice inicial*/
	int fin;	/* indice final */
};

void main(int argc,char *argv[]) {
	int i,suma,suma2,tam,error;
	pthread_t *tid;
	struct arg *param;

	if (argc != 3){printf("Uso: thr_n <int> <n_thr>\n");exit(1);}

	n=atoi(argv[1]);
	n_thr=atoi(argv[2]);

	/* reserva dinamica memoria */
	tid=calloc(n_thr+1,sizeof(pthread_t));
	param=calloc(n_thr+1,sizeof(struct arg));

	tam=n/n_thr;
	printf("Calculando S(%d) en %d threads -> tam=%d\n",n,n_thr,tam);

	/* creando threads para suma parciales */
	for (i=1;i<=n_thr;i=i+1) {
		param[i].ini=tam*(i-1)+1;
		param[i].fin=tam*i;
		error=pthread_create(&tid[i],NULL,start,&param[i]);
		if (error!=0) syserr(pthread_create);
	}

	if (n_thr*tam<n) {	/* falta sumar el resto */
		tid[0]=pthread_self();
/* printf("\tSoy el thread %d, arg=(%d,%d)\n",(int)tid[0],n_thr*tam+1,n);*/
		suma2=0;
		for (i=n_thr*tam+1;i<=n;i=i+1) suma2=suma2+1;
/* printf("\tSuma resto = %d\n",suma2);*/
		suma=suma2;
	}
	printf("Esperando terminacion threads\n");
	for (i=1;i<=n_thr;i=i+1) {
		pthread_join(tid[i],(void **)&suma2);
		suma=suma+suma2;
	}
	printf("Terminado. S(%d)=%d\n",n,suma);
}

void *start(void *p) {
	pthread_t tid;
	int tmp,i,ini,fin;

	tid=pthread_self();
	ini=((struct arg *)p)->ini;
	fin=((struct arg *)p)->fin;
/* printf("\tSoy el thread %d, arg=(%d,%d)\n",(int)tid,ini,fin);*/
	tmp=0;
	for (i=ini;i<=fin;i=i+1) tmp=tmp+1;
/* printf("\tSuma thr%d = %d\n",tid,tmp);*/
	pthread_exit((void *)tmp);
}
