/*	ej80.c	*/

#include "error.h"

main()
{
	if( -1==wait((int*)0) )
		syserr("wait");
	exit(1);
}
