	/* ej60.c
	 * Curso UNIX
	 * Mayo 1990
	 */
main()
{
	execl( "/bin/ls", "ls", "-c", 0 );
}
