/*sendsignaltome.c Práctica 4 SOPER*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void fcaptura(int s);
int main(int argc, char *argv[]){
void (* syshandler)(int);
    if (argc<2) {
        printf("Formato: ./sendsignaltome int\n");

    } else {
        int pid = getpid();
        int senyal = atoi(argv[1]);
        if (senyal != 9 && senyal != 23) {syshandler=signal(senyal, fcaptura);}
        kill(pid,senyal);
    }
}

void fcaptura(int s) {
    void (* syshandler)(int);
    printf("Capturada señal: %d\n", s);
    syshandler=signal(s, fcaptura);
}