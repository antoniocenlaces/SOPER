#include<stdio.h>
int global;

main()
{
	int local;

	local = 1;
	if( fork() == 0)
	{global = 2; local = 3;
	_exit();
	}
	printf( "global %d local %d \n", global, local);
	}
