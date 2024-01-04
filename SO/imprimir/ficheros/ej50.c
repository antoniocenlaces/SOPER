	/* Ej50.C
	 * Curso Unix.
	 * Autor: Rochkind, M.J.(1985) 'Advanced Unix Programming'.
	 * Funci'on cierra() y abre(). Implementaci'on de un sem'aforo
	 *   mediante un fichero temporal. Si el fichero no existe se crea
	 *   (el sem'aforo se CIERRA). El sistema asegura que si dos 
	 *   cesos efect'uan un creat() sobre un mismo fichero, s'olo
	 *   uno de ellos tendr'a 'exito. No asegura lo mismo de 
	 *   fstat(), que tambi'en podr'ia servir para este prop'osito.
	 *   Cuando un proceso acaba su trabajo en la zona de acceso
	 *   en exclusi'on mutua, ABRE el sem'aforo (elimina el fichero).
	 *   El m'etodo es lento, portable, y no puede ser utilizado por
	 *   el SuperUser, ya que creat() nunca le falla.
	 */
#include <string.h>
#include "error.h"

#define MAXINTENTOS	5
#define SEGUNDOS	3
#define DIRSEMAFORO	"/tmp/"

typedef enum {FALSE = 0, TRUE = 1} BOOLEAN;

extern void syserr();

static char temporal[20];
	
BOOLEAN cierra( nom_fich_sem )
char *nom_fich_sem;
{
	 int fd, intentos = 0;
	 extern int errno;
		
 	 strcpy( temporal, DIRSEMAFORO);
	 strcat( temporal, nom_fich_sem );
	 while ( (-1 == (fd = creat( temporal, 0 ))) && (errno == EACCES) ){
		if ( ++intentos >= MAXINTENTOS)
         		return FALSE;
		sleep( SEGUNDOS );
                fprintf( stderr, "\ndespierto vez %d\n", intentos );
	}
	if ( fd == -1 || close( fd ) == -1)
		syserr( "cierra" );
	return TRUE;
}

void abre(  )
{
	if ( -1 ==
 unlink( temporal))
		syserr( "abre" );
}
