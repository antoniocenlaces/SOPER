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

// La llegada de la SIGUSR1 a cualquier proceso hace cambiar la variable global senialado
// de ese proceso al valor 1.
void capturaUSR1(int n);

// Ante la señal SIGINT todos los procesos acaban de forma controlada
void capturaINT(int n);

// Pre: master es un FD abierto para lectura. Proporciona la comunicación de todos
//      los procesos en el juego con informador (el proceso que ejecuta este procedimiento).
// Post: Para cada mensaje que le llega por el FD master muestra una explicación por salida std
// Nota: Codificación de mensajes. Se usa un vector de enteros con 3 componentes:
//          - mensaje[0]: Codifica el tipo de mensaje que se ha recibido, siendo:
//                          <0>: Un nuevo proceso se une al juego. Este proceso es descendiente del creador inicial
//                          <1>: Un proceso (jugador) ha enviado la pelota de su vecino posterior
//                          <2>: Un proceso (jugador) ha recibido la pelota de su vecino anterior
//                          <3>: El proceso que inició el juego (padre inicial) se ha unido al juego
//                          <4>: Un proceso (jugador) ha sido señalado por la SIGUSR1
//          - mensaje[1]: pid del proceso (jugador) que envía el mensaje a informador
//          - mensaje[2]: ppid del proceso que creó a este jugador / valor de la pelota
void informador(int master);

// Procedimiento que ejecutan todos los jugadores
// Pre: padre es un entero con valor 1 si quien entra es el padre, 0 en caso contrario
// Post: Bucle infinito donde:
//       1.- Se espera a recibir la pelota del vecino anterior
//       2.- Si ha llegado la señal SIGUSR1 a este proceso invoca a la rutina que duplica el proceso
//       3.- Escribe a informador lo que ha recibido y lo que va a enviar
//       4.- Envía la pelota al vecino posterior
void entraEnJuego(int master, int padre);

// Pre: master es un FD abierto para escritura. Proporciona la comunicación con el proceso
//      informador.
// Post: Duplica el proceso que invoca creaNuevoJugador() y establece un pipe de comunicación
//       entre el proceso creador y el creado. Al acabar el proceso creador tiene en FD 1 un
//       canal de escritura que le comunica con el nuevo proceso. El nuevo proceso tiene en
//       FD 0 el canal donde leer de su padre y además heredó de su padre un canal (pipe)
//       por donde puede comunicar (escribir) a su vecino posterior (que antes lo era de
//       su padre)
void creaNuevoJugador(int master);

int main(){
    int i, pid;

    sigset_t myMask;
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

    // Va a ser creado un proceso informador que relatará todo lo que va ocurriendo en el juego
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
            // El juego se inicia con el envío de esta primera pelota
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
    write(master, mensaje, 12);
    int recibe, pelota;
    while (1) {
        recibe = read(0, &pelota, 4); // Me bloqueo hasta que me llegue la pelota
        if (recibe == 0) syserr("read");
        // Una pelota ha llegado. Envío mensaje a informador
        mensaje[0] = 2; // Pelota recibida
        mensaje[2] = pelota;
        write(master, mensaje, 12);
        // Voy a comprobar si he de duplicarme para crear un nuevo hijo
        if (senialado){ // senialado es varible global que cambia a 1 en la rutina de captura de SIGUSR1
            senialado = 0;
            mensaje[0] = 4; // Proceso señalado
            write(master, mensaje, 12);
            creaNuevoJugador(master);
        }
        pelota++; // Preparo pelota para mi vecino postarior
        mensaje[0] = 1; // Pelota enviada
        mensaje[2] = pelota;
        sleep(1); // Espero 1s para enviar pelota
        write(master, mensaje, 12);
        write(1, &pelota, 4);
    }
}
// Pre: master es un FD abierto para escritura. Proporciona la comunicación con el proceso
//      informador.
// Post: Duplica el proceso que invoca creaNuevoJugador() y establece un pipe de comunicación
//       entre el proceso creador y el creado. Al acabar el proceso creador tiene en FD 1 un
//       canal de escritura que le comunica con el nuevo proceso. El nuevo proceso tiene en
//       FD 0 el canal donde leer de su padre y además heredó de su padre un canal (pipe)
//       por donde puede comunicar (escribir) a su vecino posterior (que antes lo era de
//       su padre)
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
// Pre: master es un FD abierto para lectura. Proporciona la comunicación de todos
//      los procesos en el juego con informador (el proceso que ejecuta este procedimiento).
// Post: Para cada mensaje que le llega por el FD master muestra una explicación por salida std
// Nota: Codificación de mensajes. Se usa un vector de enteros con 3 componentes:
//          - mensaje[0]: Codifica el tipo de mensaje que se ha recibido, siendo:
//                          <0>: Un nuevo proceso se une al juego. Este proceso es descendiente del creador inicial
//                          <1>: Un proceso (jugador) ha enviado la pelota de su vecino posterior
//                          <2>: Un proceso (jugador) ha recibido la pelota de su vecino anterior
//                          <3>: El proceso que inició el juego (padre inicial) se ha unido al juego
//                          <4>: Un proceso (jugador) ha sido señalado por la SIGUSR1
//          - mensaje[1]: pid del proceso (jugador) que envía el mensaje a informador
//          - mensaje[2]: ppid del proceso que creó a este jugador / valor de la pelota
void informador(int master){
    int recibe, mensaje[3], n=0;
    while (1){
        recibe = read(master, mensaje, 12);
        if (recibe == 0) syserr("read");
        switch (mensaje[0]){
            case 0:
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
                n++;
                printf("Jugador Padre: %d con pid=%d, creado por: %d\n",n , mensaje[1], mensaje[2]);
                break;
            case 4:
                printf("Jugador pid=%d, ha sido señalado por SIGUSR1\n", mensaje[1]);
        }
    }
}

// La llegada de la SIGUSR1 a cualquier proceso hace cambiar la variable global senialado
// de ese proceso al valor 1.
void capturaUSR1(int n){
    senialado = 1;
}

// Ante la señal SIGINT todos los procesos acaban de forma controlada
void capturaINT(int n){
      fprintf(stderr, "\nHe recibido SIGINT\n");
      exit(0);
}
