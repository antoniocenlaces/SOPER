/*cuestion6_2.c
Cuestión 6 del grupo 2 de cuestiones*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
      char *args[4]={"ps", "-lu", "ubuntu", NULL};
      execvp(args[0], &args[0]);
}