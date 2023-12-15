#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
# include <sys/stat.h>
#include "error.h"

int main(int argc, char * argv[]) {
      int fd[2],fdf,n,m,i,tam,s=0;
      int * v;
      struct stat st;
      // m -> tamaño (nº de enteros) que va a procesar cada uno de los n procesadores
      // n -> nº de procesadores

      n=atoi(argv[2]);
      stat(argv[1], &st);

      tam = st.st_size;

      // tam -> tamaño del fichero indicado en bytes. Será múltiplo de 4.
      // atm/4 -> nº de enteros en el fichero
      m = tam / (n * 4);

      printf("Size of file: %d, n_processes: %d, bytes para cada proceso: %d, Enteros por proceso: %d\n", tam, n, m*4, m);

      v = malloc(m*4); // Reserva memoria para un vector de m enteros

      // Abre pipe para comunicar hijos con padre
      pipe(fd);

      for (i=0; i<n; i++) { 
            if (fork() == 0) {// Código del hijo
                  close(fd[0]); // Cierra ecxtremo de lectura, el hijo solo va a escribir en el pipe
                  fdf = open(argv[1], 0); // Abre el fichero para solo lectura
                  lseek(fdf, 4*m*i, 0);
                  read(fdf, v, 4*m);

                  for (i=0; i<m; i++) s = s+ v[i];

                  write(fd[1], &s, 4);
                  exit(0); // Este hijo acaba correctamente
            }
      }
      // Continúa código del padre
      close(fd[1]);
      if ((m*n*4)!=tam) {
            fdf = open(argv[1], 0);
            int l = lseek(fdf, 4*m*n, 0);
            printf("Al hacer lseek estoy con cursor en: %d\n", l);
            // m es el nº de bytes que resta leer de fichero
            m =read(fdf, v, 4*m); // 4*m es el tamaño original de v
            m = m/4;
            for (i=0; i<m; i++) s = s + v[i];
printf("Resto: %d bytes, %d enteros\n", m*4, m);
      }
      for (i=0; i<n; i++) {
            wait(NULL);
            read(fd[0], &m, 4);
            s = s + m;

      }
printf("Suma: %d\n", s);
}