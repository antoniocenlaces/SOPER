/* 143045_pr1.c
 * Ejercicio voluntario 1.
 * Autor: Antonio José González Almela (NIP 143045)
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
//      i <= 62
//      m es el número de caracteres a mostrar por salida estandard
// Post: printCaracteres muestra por salida estandar el caracter ('A' + i) m veces
void printCaracteres(int i, int m);

int main(int argc, char *argv[]) {
      if (argc != 3) {
            printf("\nUso: 143045_pr1 <n> <m>\n");
            exit(1);
      }
      int i, pid;
      // Extracción de los argumentos y conversión a int
      int n = atoi(argv[1]); // Número de procesos que se lanzarán
      int m = atoi(argv[2]); // Número de caracteres que mostrará cada uno de los n procesos
      // Comprobación si n <= 62 (número de caracteres diferentes a mostrar, igual
      // al número de procesos a lanzar)
      if (n < 63){
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
            // Proceso padre hace la tarea del proceso n
            printCaracteres(n - 1, m);
            // Una vez realizada su tarea espera que acaben todos sus descendientes
            for (i=0; i < (n - 1); i++) pid = wait(NULL);
            // Por último un salto de línea antes de acabar
            printf("\n");
      } else {
            printf("\nMáximo número de procesos a lanzar: 62. Vuelve a intentarlo.\n");
      }
}

// Pre: i es un entero que representa el número de proceso
//      i <= 62
//      m es el número de caracteres a mostrar por salida estandard
// Post: printCaracteres muestra por salida estandar el caracter ('A' + i) m veces
void printCaracteres(int i, int m){
      i = i + 'A';
      char c = (char)i;
      for (int j=0; j < m; j++) write(1, &c, 1);
}