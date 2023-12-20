/*	ej42.c	*/
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


#define DIM 100 

int main( argc, argv )
int argc; char *argv[];
{
	int  fd[DIM], contador, i;

	
	if( (fd[0]  = creat( argv[1], 0777 )) == -1 ){
		perror( "creat" ); exit(1);
	}
		
	printf( "Abro el %d\n", fd[0] );

	for( i = 1; ( fd[i] = open( argv[1], O_RDONLY )) != -1 ; i++ )
		printf( "Abro el %d\n", fd[i] );

	contador=i;
	if( errno == EMFILE )	
	   printf( "\tTotal fich. abiertos: %d\n", (contador+3) );
	else{
		perror( "open" ); exit(1);
	}

	for( i=0; i<contador; ++i)
		close( fd[i] );

	unlink( argv[1] );
}

