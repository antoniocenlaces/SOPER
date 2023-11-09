/*sendsignaltome.c Práctica 4 SOPER*/

#include <signal.h>
#include <unistd.h>


int main(int argc, char *argv[]){
int pid = getpid();
if (argc<2) {
    printf("Formato: ./sendsignaltome int");

} else {
    kill(0,atoi(argv[1]));
}
}