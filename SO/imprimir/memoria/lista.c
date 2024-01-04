#include <stdio.h>

struct registro{
int c1;
int c2;
struct registro *next;
};


void insertar (lista)
struct registro *lista;
{
struct registro *elem;
elem=(struct registro *) malloc(sizeof(struct registro));
elem->next=lista->next;
lista->next = elem;
}

void borrar(struct registro *lista){
struct registro *elem;
elem = lista->next;
lista->next=elem->next;
free(elem);
}

void main(int argc, char **argv){

struct registro lista, *aux;
int i, MAX;

MAX= atoi(argv[1]);

lista.next=NULL;
for(i=0;i<MAX;i++){
	insertar(&lista);
	insertar(&lista);
	aux=lista.next;
	insertar(&lista);
	insertar(&lista);
	borrar(aux);
}

}


