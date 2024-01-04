/* reverse.c	Invierte el contenido de un fichero.*/

#include <stdio.h>
#include <fcntl.h>
#include "error.h"

main(argc,argv)
int argc;
char *argv[];
{
	char c;
	int i, fdfnt,fddst;
	long where;

	if(argc != 3){
		printf( "Uso: %s fichero_a_invertir fichero_destino", argv[0]);
		exit(1);
	}

	if((fdfnt = open(argv[1], O_RDONLY)) == -1)
		syserr("open del primer fichero");

	if((fddst = creat(argv[2], 0600)) == -1)
		syserr("creat del segundo fichero");

	if((where=lseek(fdfnt,-1L,2)) == -1)
		syserr("lseek al final del fichero");

	while(where > 0){
		read(fdfnt, &c, 1);
		write(fddst, &c, 1);
		where=lseek(fdfnt,-2L,1);
	};
	read(fdfnt, &c, 1);
	write(fddst, &c, 1);

}
