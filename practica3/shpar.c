/*	shpar.c	*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "error.h"

int main(int argc,char *argv[]) {
      int i = 0;
      while (*argv[i] != '+') {
            i++;
      }
      argv[i] = 0;
      switch (fork()) {
            case -1:
            /* error */
                fprintf(stderr, "\nNo se puede crear proceso nuevo\n");
                syserr("fork");
            case 0:
            // Código del hijo
                  execvp(argv[1], &argv[1]);
            default:
            // Código del padre
                  execvp(argv[i+1], &argv[i+1]);
                  wait(NULL);
      }
}