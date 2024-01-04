/*	ej110.c
* Redireccionamiento who > quien_hay.tmp	*/

#include "error.h"
#include <fcntl.h>
#include <stdio.h>

main()
{
	int fd;
	if( (fd= creat( "quien_hay.tmp", 0640) )== -1)
	   if( errno == EACCES )
	   {
	     printf( "\nfichero existente sin permiso escritura\n" );
	     exit( 0 );
	   }

	   else
	     syserr( "creat" );
	
	close( fd );
	close( 1 );

	fd = open( "quien_hay.tmp", O_WRONLY );
	if( fd == 1 )
	  fprintf( stderr ,"Salida Redireccionada\n");
	else
	{
	  fprintf( stderr, "Virus\n" );
	  exit( 1 );
	}
	execl( "/bin/who", "who", 0 );
	syserr( "execl" );
}




