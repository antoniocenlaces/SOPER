/* lista_mejorada.c
*/
#include <stdio.h>


struct registro{
int c1;
int c2;
struct registro *next;
};

struct registro *libres=NULL;

struct registro * mi_malloc(){
int i;
struct registro *aux;

if (libres==NULL) {
        libres=(struct registro *) malloc(1000*sizeof(struct registro));
        for (i=0;i<999;i++)libres[i].next=&libres[i+1];
        libres[999].next=NULL;
}
aux=libres;
libres=libres->next;
return aux;
}

void mi_free(struct registro * elem){
elem->next=libres;
libres=elem;
}

void insertar (lista)
struct registro *lista;
{
struct registro *elem;
elem=(struct registro *) mi_malloc();
elem->next=lista->next;
lista->next = elem;
}

void borrar(struct registro *lista){
struct registro *elem;
elem = lista->next;
lista->next=elem->next;
mi_free(elem);
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

