/* 143045_pr4.c
 * Ejercicio voluntario 4.
 * Integral por método Riemann de 4/(1+x^2) en el intervalo [0, 1].
 * Autor: Antonio José González Almela (NIP 143045)
 * Uso: 143045_pr4
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "error.h"

struct arg {
    int ini;
    int fin;
    double res;
};
int n, m;

void *start(void *p);

void main(int argc, char * argv[]){
    int i, error, trozos;
    double res, segmento, halfs, x;
    pthread_t * tid;
    struct arg * param;

    if (argc != 3) {printf("Uso: 143045_pr4 <n> <m>\n");exit(1);}

    n = atoi(argv[1]); // Número de segmentos en que se divide el intervalo [0, 1]
    m = atoi(argv[2]); // Número de threads que ejecutan la integral

    // Pedimos memoria dinámica para el vector tid y param
    tid = calloc(m, sizeof(pthread_t));
    param = calloc(m, sizeof(struct arg));

    trozos = n / m; // Cantidad de segmentos que va a procesar cada hilo

    printf("Calculando integral en %d trozos con %d hilos. Cada hilo hace %d segmentos\n", n, m, trozos);

    // Creación de hilos de cálculo
    for(i = 0; i < m; i++){
        param[i].ini = trozos * i;
        param[i].fin = trozos * (i + 1);
        error = pthread_create(&tid[i], NULL, start, &param[i]);
        if (error != 0) syserr("pthread_create");
    }
    res = 0.0;
    segmento = 1.0 / (double)n;
    halfs = 1.0 / (double)(n * 2);
    if ((trozos * m) < n) {
        for (i = trozos * m; i < n; i++){
            x = (i * segmento) + halfs;
            res = res + segmento * 4.0 / (1.0 + (x * x));
        }
    }

    for (i=0;i<m;i=i+1) {
        pthread_join(tid[i],NULL); 
        res=res+param[i].res; 
    } 
printf("Terminado.PI=%.9f\n", res);

}

void *start(void *p){
    int i, ini, fin;
    double tmp, segmento, halfs, x;
    tmp = 0.0;
    segmento = 1.0 / (double)n;
    halfs = 1.0 / (double)(n * 2);
    ini = ((struct arg *) p)->ini;
    fin = ((struct arg *) p)->fin;

    for (i = ini; i < fin; i++){
        x = (i * segmento) + halfs;
        tmp = tmp + segmento * 4.0 / (1.0 + (x * x));
    }
    ((struct arg *) p)->res = tmp;
    pthread_exit(NULL);
}