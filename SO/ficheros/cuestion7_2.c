/*cuestion7_2.c
* Programa en C para crear un fichero de 10KB
* con mínimo número de llamadas al sistema
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
      int fd = creat("file", 0666);
      lseek(fd, 10239, SEEK_SET);
      char c = '1';
      write(fd, &c, 1);
      // write(fd, "1", 1);
}