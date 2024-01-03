/*cuestion6_2.c
Cuestión 6 del grupo 2 de cuestiones
Programa C que ejecuta comando ps -lu ubuntu
con execvp()
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
      // char *args[4]={"ps", "-lu", "ubuntu", NULL};
      char *args[4];
      args[0] = "ps";
      args[1]= "-lu";
      args[2]= "ubuntu";
      args[3]=NULL;
      execvp(args[0], &args[0]);
}