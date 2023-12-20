	/* ej70.c
	 * Curso UNIX
	 * Mayo 1990
	 * Probar imprimiendo el pid (int) del padre y del hijo.
	 */
#include <sys/types.h>
	/* para funciones rel. con el pid; depende del sistema.
	 */
#include <stdio.h>
#include "error.h"

main()
{
	pid_t getpid();
	int pidh;

	printf( "Inicio prueba\n" );
	fflush( stdout );

	if ( (pidh=fork())==0 ){
		fprintf( stderr, "\n\tSoy el hijo: %d\n", getpid() );
		fprintf( stdout, "\n\tFork me devuelve: %d\n", pidh );
		exit( 0 );
	}

	fprintf( stderr, "Antes de sleep\n");
	sleep( 1 );
	if( -1 ==wait(NULL))
		syserr("wait");

	fprintf( stdout, "\n\tSoy el padre: %d\n", getpid() );
	fprintf( stdout, "\n\tFork me devuelve: %d\n", pidh );
}
