	/* ej10.c
	 * Mayo 1990.
	 * Copia la entrada en la salida. <fcntl.h> contiene
	 *   O_RDONLY. "error.h" contiene syserr (cfr. Rochkind. M.J 
	 *   (1985).- Advanced UNIX Programming. Prentice Hall.)
	 */
#include <fcntl.h>
#include "error.h"

#define BUFSIZE 2048

main( argc, argv )
int argc; char *argv[];
{
	int fdfnt, fddst;
	extern void copia();

	if (argc != 3){
		printf( "Uso: %s fich1 fich2 ", argv[0] );
		exit( 1 );
	}

	if ( (fdfnt = open( argv[1], O_RDONLY )) == -1 )
		syserr( "open" );

	if ( (fddst = creat( argv[2], 0666 )) == -1 ) 
		syserr( "creat" );

	copia( fdfnt, fddst );

	exit( 0 );
}

void copia( fnt, dst )
int fnt, dst;
{	
	int cuenta;
	char buf[BUFSIZE];

	while ( (cuenta = read( fnt, buf, sizeof( buf ) )) > 0 )
		write( dst, buf, cuenta );
}
