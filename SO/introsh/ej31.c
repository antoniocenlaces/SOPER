	/* ej31.c
	 * Curso UNIX. Victor Vi·als.
	 * Mayo 1990.
	 * Imprime variables de entorno. 
	 */

#include <stdio.h>

main( argc, argv, envp )
int argc;
char *argv[];
char *envp[];

{
	int i;

	for ( i = 0; envp[i] != NULL; i++ )
		printf( "%s\n", envp[i] );
	exit( 0 );
}

