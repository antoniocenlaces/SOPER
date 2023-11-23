/*micat3.c Práctica 5 SOPER*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include "../error.h"

#define BUFF 1024

int main(int argc, char *argv[]) {
  int fdR = 0;
  long pos;
  if (argc == 2) {
      close(1);
      fdR = open(argv[1], O_RDWR);
      pos = lseek(fdR, 0, SEEK_END);
  }
  char c[BUFF]; int n;
  if ( (n=read(0, c, BUFF)) == -1 ) syserr("read");
  while (n>0) {
      if ( (n=write(1, c, n)) == -1 ) syserr("write");
      if ( (n=read(0, c, BUFF)) == -1 ) syserr("read");
  }
}