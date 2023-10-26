#include <iostream>
#include <unistd.h>
#include <string>
#include <stdio.h>
using namespace std;

int main(){
int id;
cout << "Comienza la ejecución\n";
id=fork();
if (id==0){
             cout << "Soy el hijo\n";
             cout << "Mi PID es: " + to_string(getpid()) + "\n";
             cout << "Mi PID de mi padre es: " + to_string(getppid()) + "\n";
             }
             else{
             cout << "Soy el padre\n";
             cout << "Mi PID es: " + to_string(getpid()) + "\n";
             cout << "Mi PID de mi padre es: " + to_string(getppid()) + "\n";
            }
cout << "Termina la ejecución\n" << endl;
exit(0);
}  