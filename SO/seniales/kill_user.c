/* kill_user.c
 * Intento de terminar todos los procesos de un usuario excepto su bash.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "error.h"

void captura(int n);

int main(){
    struct sigaction myAction;
    myAction.sa_handler = captura;
    sigemptyset(&myAction.sa_mask);
    int i = sigaction(SIGUSR1, &myAction, NULL);
    kill(0, SIGUSR1);
}

void captura(int n){
    printf("Yo soy %d con GID %d\n", getpid(), getgid());
}
