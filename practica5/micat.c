/*micat.c Práctica 5 SOPER*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "../error.h"

#define BUFF 1024

int main(int argc, char *argv[]) {
  char c[BUFF]; int n;
  if ( (n=read(0, c, BUFF)) == -1 ) syserr("read");
  while (n>0) {
      if ( (n=write(1, c, n)) == -1 ) syserr("write");
      if ( (n=read(0, c, BUFF)) == -1 ) syserr("read");
  } 
}