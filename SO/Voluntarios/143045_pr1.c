/* 143045_pr1.c
 * Ejercicio voluntario 1.
 * Uso: 143045_pr1 <n> <m>
 * int n: número de procesos a lanzar
 * int m: número de caracteres que mostrará cada uno de los n procesos
 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "error.h"

// Pre: i es un entero que representa el número de proceso
//      m es el número de caracteres a mostrar por salida estandard
// Post: printCaracteres muestra por salida estandar el caracter ('A' + i) m veces
void printCaracteres(int i, int m);

int main(int argc, char *argv[]) {
      if (argc != 3) {
            printf("\nUso: 143045_pr1 <n> <m>\n");
            exit(1);
      }
      int i, pid;
      i = i + 'A';
      printf("Caracter calculado: %c\n",i);
      char c = (char)i;
      write(1,&c,1);
      c='\n';
      write(1,&c,1);

      int n = atoi(argv[1]); // Número de procesos que se lanzarán
      int m = atoi(argv[2]); // Número de caracteres que mostrará cada uno de los n procesos
      // Comprobación si n <= 61 (número de caracteres diferentes a mostrar, igual
      // alnúmero de procesos a lanzar)
      if (n < 62){
            // Proceso padre será el último en hacer su trabajo (proceso n)
            // Bucle para lanzar (n-1) procesos
            for (i=0; i < (n - 1); i++) {
                  switch (pid = fork()){
                        case -1: /* error */
                              fprintf(stderr, "\nNo se puede crear proceso nuevo\n");
                              syserr("fork");
                        case 0: // Código a ejecutar por el hijo i
                              printCaracteres(i, m);
                              exit(0);
                  }
            }
      } else {
            printf("\nMáximo número de procesos a lanzar: 61. Vuelve a intentarlo.\n");
      }
}

// Pre: i es un entero que representa el número de proceso
//      i <= 61
//      m es el número de caracteres a mostrar por salida estandard
// Post: printCaracteres muestra por salida estandar el caracter ('A' + i) m veces
void printCaracteres(int i, int m){
      i = i + 'A';
      char c = (char)i;
      for (int j=0; j < m; j++) write(1, &c, 1);
}