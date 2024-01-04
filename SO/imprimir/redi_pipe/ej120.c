       /* ej120.c
        * who | sort hijo a padre
        * cfr ej1201.c
        */

#include "error.h"

main()
{
  int fd[2];

  pipe( fd );

      switch( fork() )
	{
	case -1:
	  syserr( "fork" );
        case 0:
	  close( fd[0] );
	  close( 1 );
	  dup( fd[1] );
	  close( fd[1] );

	  execlp( "who", "who", 0 );

	default:
	  close( fd[1] );
	  close( 0 );
	  dup( fd[0] );
	  close( fd[0] );
	}
      execlp( "sort", "sort", 0 );
}




