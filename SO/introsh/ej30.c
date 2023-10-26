	/* ej30.c
	 * Curso UNIX. Victor Vi·als.
	 * Mayo 1990.
	 * Imprime variables de entorno. Puede hacerse lo mismo con 
	 *      envp[] (cfr. prog. incluido al final).
	 */
#include <stdio.h>

/*extern char **environ;

main( argc, argv )
int argc; char *argv[];
{
	int i;

	for ( i = 0; environ[i] != NULL; i++ )
		printf( "%s\n", environ[i] );
	exit( 0 );
}*/
main( argc, argv, envp )
int argc; char *argv[], *envp[];
{
	int i;

	for ( i = 0; envp[i] != NULL; i++ )
		printf( "%s\n", envp[i] );
	exit( 0 );
}
