/* reverse.c	Invierte el contenido de un fichero.*/

#include <stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/mman.h>	/* mmap() */
#include	<fcntl.h>
#include "error.h"

#ifndef	MAP_FILE	/* 44BSD defines this & requires it to mmap files */
#define	MAP_FILE	0	/* to compile under systems other than 44BSD */
#endif



main(argc,argv)
int argc;
char *argv[];
{
	int fdfnt;
	long fsize;
	char *src;

	if(argc != 2){
		printf( "Uso: %s fichero_a_invertir", argv[0]);
		exit(1);
	}

	if((fdfnt = open(argv[1], O_RDONLY)) == -1)
		syserr("open del primer fichero");

	if((fsize=lseek(fdfnt,-1L,2)) == -1)
		syserr("lseek al final del fichero");

	if ( (src = mmap(0, fsize, PROT_READ,
		 MAP_FILE | MAP_SHARED, fdfnt, 0)) == -1)
			syserr("mmap error for input");
	fsize--;
	while(fsize >= 0){
		write(1, &src[fsize], 1);
		fsize--;
	};

}
