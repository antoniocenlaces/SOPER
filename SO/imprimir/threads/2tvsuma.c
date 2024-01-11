/*2tvsuma.c*/
#include <pthread.h>
#include "../sistemas/error.h"

#define MAX 10

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

void main(){

int suma1, suma2;
struct registro r1,r2;
pthread_t tid1, tid2;
int j;

for (j=0;j<MAX;j++){
	V2[j]=j;
	V3[j]=0;
}

r1.indice=0;
r1.size=MAX/2;
r2.indice=MAX/2;
r2.size=MAX/2;

if (pthread_create(&tid1, NULL, func, &r1) != 0) syserr("crear thread");
if (pthread_create(&tid2, NULL, func, &r2) != 0) syserr("crear thread");

pthread_join(tid1, (void **)&suma1);
pthread_join(tid2, (void **)&suma2);


printf("Terminado. Suma= %d \n", suma1+suma2);

}

