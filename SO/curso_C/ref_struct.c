#include <stdio.h>

int main(){
    struct ALFA {
        int val;
        char cadena[10];
    };
    struct ALFA *a;

    // a->val=8;
    // a->cadena[2]='j';
    struct ALFA b;
    b.val=198876;
    b.cadena[0]='m';
    a = &b;
    printf("Recuperado por a val= %d cadena[0]=%c\n",a->val,a->cadena[0]);
    a->val=1024;
    a->cadena[1]='r';
    printf("NUEVO val= %d cadena[0]=%c\n",a->val,a->cadena[1]);
    return 0;
}