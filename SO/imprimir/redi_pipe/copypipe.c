#include "error.h"
/*copypipe.c*/
main(argc,argv)
int argc;
char *argv[];
{
	int fpipe[2], file, n;
	char buf[512];

	if(argc!=3) syserr("El numero de parametros no es correcto");

	pipe(fpipe);

	switch(fork()){
	case -1: syserr("fork");
	case  0: 
		file=open(argv[1],0);
		while((n=read(file,buf,500))!=0){
			n=write(fpipe[1],buf,n);
			printf("Leidos %d caracteres\n",n);
		}
		printf("Fichero leido\n");
		break;
	default: 
		close(fpipe[1]);
		file=creat(argv[2],0600);
		while((n=read(fpipe[0],buf,512))!=0){
			printf("Escritos %d caracteres\n",n);
			write(file,buf,n);
		}
		printf("Fichero copiado\n");
	}
	exit(0);
}

