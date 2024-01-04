/*	ej90.c */
#include <stdio.h>
main(argc, argv)
int argc; 
char *argv[];
{	int pf;
	int n;			
	char *s = "\n\tParametros neos\n";
	char buf[BUFSIZ];
	if( argc < 2 || (pf = open( argv[1], 2 )) == -1 ){
	    write( 2, s, strlen(s)); 
	    exit(1);
	}

	while( n = read(pf, buf, sizeof(buf))) 
	      write( 2 , buf, n);
	close ( pf );
}

