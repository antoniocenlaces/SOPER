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
	int fdfnt, fddst;
	long fsize, cont;
	char *src, *dst;

	if(argc != 3){
		printf( "Uso: %s fichero_a_invertir fichero_salida", argv[0]);
		exit(1);
	}

	if((fdfnt = open(argv[1], O_RDONLY)) == -1)
		syserr("open del primer fichero");

	if((fsize=lseek(fdfnt,-1L,2)) == -1)
		syserr("lseek al final del fichero de entrada");

	if((fddst = creat(argv[2], 0600)) == -1)
		syserr("creat del segundo fichero");
	close(fddst);	
	if((fddst = open(argv[2], O_RDWR)) == -1)
		syserr("open del primer fichero");

	if ( (src = mmap(0, fsize, PROT_READ,
		 MAP_FILE | MAP_SHARED, fdfnt, 0)) == -1)
			syserr("mmap error for input");

	if(lseek(fddst,fsize,0) == -1)
		syserr("lseek al final del fichero de salida");
	write(fddst,"algo",1);

	if ( (dst = mmap(0, fsize, PROT_WRITE,
		 MAP_FILE | MAP_SHARED, fddst, 0)) == -1)
			syserr("mmap error for output");
	fsize--;
	cont=0;
	while(fsize >= 0){
		dst[cont]=src[fsize];
		fsize--;
		cont++;
	};

}
