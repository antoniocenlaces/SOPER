/*	ej61.c	*/

#include "error.h"
main()
{
	execl( "/bin/ls", "ls", "-c", 0 );
	syserr( "execlp" );
}
