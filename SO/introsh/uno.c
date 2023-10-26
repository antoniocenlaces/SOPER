      /* uno.c
       * Tutorial de C
       * Arquitectura y Tecno. de Computadores
       * 
       * Primer programa en C. Un programa en C consta, como minimo,
       * de una funcion ("main(){}").
       * "main" es el nombre de la funcion.
       * Los argumentos se indican entre "()" (en este programa no los hay).
       * "{}" encierran el cuerpo de la funcion.
       * Consultar en un libro de C o en man como es y que hace printf.
       *
       * Los comentarios no pueden anidarse
       *
       *
       * En C no pueden definirse funciones dentro de funciones.
       * En este ejemplo, main() usa una funcion que se llama funcion()
       * para ver como se declaran y usan funciones. funcion() no tiene
       * argumentos. Observar que esta definida fuera de main().
       */


#include <stdio.h>  /* standard input/output library */


void funcion(); /* indica que main() utilizara una funcion
			  denominada asi, y declarada en este fichero
			  Esto es una ALUSION
			*/

main()
{

  printf( "\n\tHello, World!\n" ); /* funcion de la stdio library */

  funcion();  /* Esto es una LLAMADA a la funcion funcion() */
}

                      /* Esto es una DECLARACION de una funcion,
		         que imprime un mensaje, y que devuelve "void"
			 (en C todas las funciones devuelven algo)
		       */

void funcion(){
  printf( "\nHello, World, Again!\n" ); 
} 


