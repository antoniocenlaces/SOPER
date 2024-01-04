#include <pthread.h>
#include "../sistemas/error.h"

#define MAX 120000000

int V1[MAX], V2[MAX], V3[MAX];

struct registro{
int indice;
int size;
};

void * func(p)
void * p;
{
int i, suma=0;
int inicio,elementos;

inicio=((struct registro *)p)->indice;
elementos=((struct registro *)p)->size;

for (i=inicio;i<inicio+elementos;i++){
	V1[i]=V2[i]+V3[i];
	suma+=V1[i];
}
pthread_exit((void *)suma);

}

void main(int argc, char** argv){

int vsuma[100];
struct registro r[100];
pthread_t tid[100];
int j, hilos, trozo, total=0;

hilos=atoi(argv[1]);

/*
for (j=0;j<MAX;j++){
	V2[j]=j;
	V3[j]=0;
}
*/

trozo = MAX/hilos;

for(j=0;j<hilos;j++){
	r[j].indice=j*trozo;
	r[j].size=trozo;
	if (pthread_create(&tid[j], NULL, func, &r[j]) != 0) syserr("crear thread");
}

if(hilos*trozo<MAX)
	for(j=hilos*trozo;j<MAX;j++){
		V1[j]=V2[j]+V3[j];
		total+=V1[j];
	}

for(j=0;j<hilos;j++){
	pthread_join(tid[j], (void **)&vsuma[j]);
	total+=vsuma[j];
}



printf("Terminado. Suma= %d \n", total);

}

