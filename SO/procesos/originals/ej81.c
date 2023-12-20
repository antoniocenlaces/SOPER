/*	ej81.c	*/

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "../error.h"

int main()
{ 
    int pid;

    switch(fork()) {
        case -1:
    	    syserr("fork");
        case 0:
            pid = getppid();
            printf("pid padre antes = %d\n",pid);
            sleep(6); 
            pid = getppid();
            printf("pid padre despues = %d\n",pid);
            exit(15);
        default:
            sleep(2);
            exit(0);
    } 
}
