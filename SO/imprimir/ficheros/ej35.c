	/* ej35.c SisOps PostInfo91
	 * Ejemplo de utilizacion de stat.
	 * Simplif. a partir de Rochkind, J(1985):67-68.
	 */
#include <stdio.h>
#include "error.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

main( argc, argv )
int argc; char *argv[];
{
	struct stat buf;

	if (stat( argv[1], &buf ) == -1)
		syserr( "stat" );
	else
		dspstatus( buf );
}
/*Anadir aqui funcion dspstatus() de Rochkind pp. 67-68*/
