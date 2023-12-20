#include "error.h"

main()
{
	execlp("ls", "ls", "-c", 0);
	syserr("execlp");
}
