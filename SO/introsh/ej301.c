	/* ej30.c
	 * Curso UNIX. Victor Vi·als.
	 * Mayo 1990.
	 * Imprime argumentos de la funcion main() 
	 */

#include <stdio.h>

main( argc, argv )
int argc; char *argv[];
{
	int i;

	for ( i = 0; i<argc; i++ )
		printf( "%s\n", argv[i] );
	exit( 0 );
}
