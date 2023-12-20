/*	ej82.c	*/

#include "error.h"

main()
{ 
int pid;
int estado;
switch(fork()) { 
	case -1:/*error*/
		syserr("fork");
	case 0 :/*hijo*/
		sleep(2); 
		printf("Soy el hijo y me muero...\n");
		exit(15);
	default:/*padre*/
		sleep(10);/*damos tiempo a que el hijo termine y se 
				quede zombie un rato.*/
		if( -1 == wait(&estado) )
			syserr( "wait" );
		printf("Estado hijo = %x\n", estado);
		exit(0);
	} 
}

