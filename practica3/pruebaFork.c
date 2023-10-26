#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>



int main(){
int id;
printf("Comienza la ejecución\n");
id=fork();
if (id==0){
             printf("Soy el hijo\n");
             printf("Mi PID es: %d\n",  getpid());
             printf("Mi PID de mi padre es: %d\n", getppid());
             }
             else{
             printf("Soy el padre\n");
             printf("Mi PID es: %d\n", getpid());
             printf("Mi PID de mi padre es: %d\n", getppid());
             wait(&id);
            }
printf("Termina la ejecución\n");
exit(0);
}  