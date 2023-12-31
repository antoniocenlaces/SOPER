/* 143045_pr3.c
 * Ejercicio voluntario 3.
 * Solución al ejercicio 37 de la colección d eproblemas.
 * Autor: Antonio José González Almela (NIP 143045)
 * Uso: 143045_pr3
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "error.h"

// Variables Globales
// Serán usadas para comunicar desde la función de captura de señal con el main de su proceso.
int senialado = 0;
int termina = 0;

void capturaUSR1(int n);
void capturaINT(int n);
void informador(int master);
void entraEnJuego(int master, int padre);
void creaNuevoJugador(int master);

int main(){
    int i, pid;

    sigset_t myMask,oldMask;
	struct sigaction myAction1, myAction2;

    // Programación de la captura de señales
    // Todo comportamiento ante señales programado en este proceso (padre) será heredado por sus hijos
    // SIGUSR1
    myAction1.sa_handler = capturaUSR1;
    sigemptyset(&myAction1.sa_mask);
    myAction1.sa_flags = SA_RESTART;
    i = sigaction(SIGUSR1, &myAction1, NULL);
    if (i==-1) syserr("sigactionUSR1");
    // SIGINT
    myAction2.sa_handler = capturaINT;
    sigemptyset(&myAction2.sa_mask);
    i = sigaction(SIGINT, &myAction2, NULL);
    if (i==-1) syserr("sigactionINT");

    // Va a sre creado un proceso informador que relatará todo lo que va ocurriendo en el juego
    // Todos los procesos tendrán el canal de escritura master para comunicar con el informador
    // Creamos un pipe asociado a master[2] que se heredará en todos los procesos a partir de ahora
    int master[2];
    pipe(master);
    switch(fork()){ // Creación del proceso informador
        case -1:
            syserr("fork");
        case 0: // Código del proceso informador
            close(master[1]); // Cierra el extremo de escritura (W) de este pipe
            informador(master[0]); // En master[0] este proceso podrá escuchar los mensajes
                                   // de los participantes en el juego
        default:
            close(master[0]); // Cierra extremo de lectura (R)
                              // A partir de aquí todos los procesos tienen abierto master[1]
                              // donde pueden escribir al informador
    }
    // Inicia proceso de creación del primer hijo (es diferente que los hijos sucesivos)
    // El proceso padre (este proceso) también entrará en el juego
    int pelota, fd1[2], fd2[2]; // pipes para comunicar con el primer hijo del juego
    pipe(fd1);
    pipe(fd2);
    pid = fork();
    switch (pid){
        case -1:
            syserr("fork");
        case 0: // Código del primer hijo
            // Este proceso lee mensajes de su vecino anterior por fd2
            // y escribe mensajes a su vecino posterior por fd1
            close(fd1[0]); // Cierra lectura de fd1
            close(1);
            dup(fd1[1]); // Ahora file descriptor (FD) 1 es el extremo de escritura de fd1
            close(fd1[1]);
            close(fd2[1]); // Cierra escritura de fd2
            close(0);
            dup(fd2[0]); // Ahora el FD 0 está asociado al extremo de lectura de fd2
            close(fd2[0]);
            entraEnJuego(master[1], 0); // Un hijo entra en el juego
        default:
            // Este proceso lee mensajes de su vecino anterior por fd1
            // y escribe mensajes a su vecino posterior por fd2
            close(fd1[1]); // Cierra escritura (W) en fd1
            close(0);
            dup(fd1[0]); // Ahora el FD 0 está asociado al extremo de lectura de fd1
            close(fd1[0]);
            close(fd2[0]); // Cierra lectura de fd2
            close(1);
            dup(fd2[1]); // Ahora FD 1 es el extremo de escritura de fd2
            close(fd2[1]);

            // Proceso de comunicación con el primer vecino de juego establecido
            // Ahora inicio enviándole la primera pelota
            pelota = 1;
            write(1, &pelota, 4);
            entraEnJuego(master[1], 1); // El padre entra en el juego
    }
}

// Procedimiento que ejecutan todos los jugadores
// Pre: padre es un entero con valor 1 si quien entra es el padre, 0 en caso contrario
// Post: Bucle infinito donde:
//       1.- Se espera a recibir la pelota del vecino anterior
//       2.- Si ha llegado la señal SIGUSR1 a este proceso invoca a la rutina que duplica el proceso
//       3.- Escribe a informador lo que ha recibido y lo que va a enviar
//       4.- Envía la pelota al vecino posterior
void entraEnJuego(int master, int padre){
    int mensaje[3];
    if (padre) mensaje[0] = 3;
        else mensaje[0] = 0;
    mensaje[1] = getpid();
    mensaje[2] = getppid();
    fprintf(stderr, "Alguien acaba de entrar en juego getpid(): %d y su getppid(): %d\n", mensaje[1], mensaje[2]);
    write(master, mensaje, 12);
    int recibe, pelota;
    while (1) {
        recibe = read(0, &pelota, 4);
        if (recibe == 0) syserr("read");
        mensaje[0] = 2;
        mensaje[2] = pelota;
        write(master, mensaje, 12);
        // He recibido la pelota y voy a comprobar si he de duplicarme para crear un nuevo hijo
        if (senialado){
            senialado = 0;
            fprintf(stderr, "Proceso %d ha sido señalado\n", getpid());
            creaNuevoJugador(master);
        }
        pelota++;
        mensaje[0] = 1;
        mensaje[2] = pelota;
        sleep(1);
        write(master, mensaje, 12);
        write(1, &pelota, 4);
    }
}

void creaNuevoJugador(int master){
    // Soy el padre que se duplica para crear un nuevo jugador
    int nd[2];
    pipe(nd);
    int pid = fork();
    switch (pid){
        case -1:
            syserr("fork");
        case 0: // Nuevo jugador
        close(0); // Cierra el extremo de lectura por donde su padre recibía mensajes
        close(nd[1]); // Cierra extremo W de pipe nd
        dup(nd[0]); // FD 0 es el extremo de lectura de pipe nd
        close(nd[0]);
        entraEnJuego(master, 0);
    default:
        close(nd[0]); // Cierra extremo R de nd
        close(1);
        dup(nd[1]); // FD 1 es el extremo de escritura de pipe nd
        close(nd[1]);
    }
}

void informador(int master){
    int recibe, mensaje[3], jugadores[50], n=0;
    while (1){
        recibe = read(master, mensaje, 12);
        if (recibe == 0) syserr("read");
        switch (mensaje[0]){
            case 0:
                jugadores[n] = mensaje[1];
                n++;
                printf("Nuevo jugador: %d con pid=%d, creado por: %d\n",n , mensaje[1], mensaje[2]);
                break;
            case 1:
                printf("Jugador pid=%d, envía pelota: %d\n", mensaje[1], mensaje[2]);
                break;
            case 2:
                printf("Jugador pid=%d, recibe pelota: %d\n", mensaje[1], mensaje[2]);
                break;
            case 3:
                jugadores[n] = mensaje[1];
                n++;
                printf("Jugador Padre: %d con pid=%d, creado por: %d\n",n , mensaje[1], mensaje[2]);
        }
    }
}

void capturaUSR1(int n){
    senialado = 1;
}

void capturaINT(int n){
      fprintf(stderr, "\nHe recibido SIGINT\n");
      exit(0);
}