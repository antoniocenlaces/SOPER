#include <stdio.h>

void main(){
    static int ar[4] = {1, 2, 3, 4};
    int *p;
    p=&ar[0];
    printf("\nEl primer elemento de ar es: %d\n", *p);
    printf("\nEl segundo elemento de ar es: %d\n", *(p+1));
    printf("\nDirecciones. p contiene: %x\n", p);
    printf("\nar contiene: %x\n", ar);
    printf("\n&ar[0] contiene: %x\n", &ar[0]);
}