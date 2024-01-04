/* reverse.c	Invierte el contenido de un fichero.*/

#include <stdio.h>
#include <fcntl.h>
#include "error.h"

main(argc,argv)
int argc;
char *argv[];
{
	char c;
	int i, fdfnt;
	long where;

	if(argc != 2){
		printf( "Uso: %s fichero_a_invertir", argv[0]);
		exit(1);
	}

	if((fdfnt = open(argv[1], O_RDONLY)) == -1)
		syserr("open del primer fichero");

	if((where=lseek(fdfnt,-1L,2)) == -1)
		syserr("lseek al final del fichero");

	while(where > 0){
		read(fdfnt, &c, 1);
		write(1, &c, 1);
		where=lseek(fdfnt,-2L,1);
	};
	read(fdfnt, &c, 1);
	write(1, &c, 1);

}
