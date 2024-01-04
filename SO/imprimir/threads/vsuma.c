
#define MAX 100000000

int V1[MAX], V2[MAX], V3[MAX];

void main(){

int i, suma;

for (i=0;i<MAX;i++){
	V1[i]=V2[i]+V3[i];
	suma+=V1[i];
}
printf("Terminado. Suma= %d \n", suma);

}

