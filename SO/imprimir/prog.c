/* Programa que ejecuta un comando descrito en fichero de
   control (pasado como argumento). El fichero es de 4 lineas
	 (1000 char maximo) con el formato
	 comando y argumentos (entre 0 y 100)
	 fichero entrada
	 fichero salida
	 fichero errores
	 
	 El programa debe lanzar un hijo que ejecute el comando con
	 sus argumentos y redireccionando sus fd 0,1 y 2 a los
	 ficheros indicados
	 */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "error.h"

int leerlinea(int,char *);

int main(int argc,char *argv[]) {
	int pid,fd,i;
	char com1[1001],fin[1000],fout[1000],ferr[1000];
	char *argt1[100];

	if (argc!=2) {printf("Uso p1 <fcontrol>\n");exit(1);}
	
	fd=open(argv[1],O_RDONLY);
	if (fd==-1) {printf("Error open %s\n",argv[1]);exit(1);}

	/* lectura fcontrol */
	leerlinea(fd,com1);
	leerlinea(fd,fin);
	leerlinea(fd,fout);
	leerlinea(fd,ferr);
	close(fd);

	/* proceso comando 1 */
	argt1[0]=strtok(com1," ");
	i=0;
	do {	
		i = i+1;
		argt1[i] = strtok(NULL," ");	
	} while (argt1[i] != NULL);


	/* ejecucion comando */
	pid=fork();
	if (pid==0)	{	/* hijo */
		/* redirecciones */
		close(0);
		fd=open(fin,O_RDONLY);
		if (fd!=0) {printf("Error redireccion stdin\n");exit(1);}
		close(1);
		fd=creat(fout,0777);
		if (fd!=1) {printf("Error redireccion stdout\n");exit(1);}
		close(2);
		fd=creat(ferr,0777);
		if (fd!=2) {printf("Error redireccion stderr\n");exit(1);}

		execvp(argt1[0],&argt1[0]);
		syserr("execvp");
		exit(1);
	}
	wait(NULL);
}

int leerlinea(int fd,char *s) {
/* lee caracter a caracter del fichero fd hasta encontrar '\n' 
	y la almacena	en s (terminado en '\0').
	Devuelve el numero de caracteres realmente leidos */

	int i,j;
	char c;

	j=0;
	i=read(fd,&c,1);
	while ((i!=0)&&(c!='\n')) {
		s[j]=c;
		j=j+1;
		i=read(fd,&c,1);
	}
	s[j]='\0';
	return(j);
}
