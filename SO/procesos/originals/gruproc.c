      /* gruproc.c 
       * Curso Unix
       * El padre se hace lider de su propio grupo
       * mediante setpgrp()
       * Crea 10 hijos
       * Los creados en iteraciones impares son convertidos
       * en lideres (independizados)
       * Se envia una SIGINT a todos los procesos del grupo,
       * que afectara a todos los no independizados incluido el
       * el propio lider del grupo (el mensa final no se imprime).
       */

#include<signal.h>
#include<stdio.h>

main(){
  register int i;
  
  setpgrp(); 
  for( i=0; i<10; i++){
    if( fork() == 0 ){
      if( i & 1 ) setpgrp();
      printf( "\n pid = %d, pgrp = %d\n", getpid(), getpgrp() );
      pause();
    }
  }
  kill( 0, SIGINT );
  printf("Mensa\n");
  fflush(stdout);
}
