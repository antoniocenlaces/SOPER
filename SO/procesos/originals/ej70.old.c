/*
 *ej701.c
 *Prueba fork().
 */

#include<stdio.h>

 main()
{
	int pid;
	fprintf( stderr, "Inicio Prueba\n" );
	pid = fork();
	fprintf( stderr, "Soy el proc. con fork= %d \n", pid );
	exit(0);
}

