#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char * argv[]) {
      if (argc != 2) { printf("Uso: %s <fichero>\n", argv[0]); exit(1);}

      int fd = creat(argv[1], 0777);

      for (int i=-3000; i<=3000; i++) {
            write(fd, &i, sizeof(i));
      }
      close(fd);
}