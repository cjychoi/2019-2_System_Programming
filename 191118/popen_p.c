/* popen.c - a versionof the Unix poen() library function

*/

#include	<stdio.h>
#include	<signal.h>
#include	<stdlib.h>
#include	<unistd.h>

#define	READ	0
#define	WRITE	1

FILE *popen(const char *command, const char *mode)
{
	int	pfp[2], pid;		/* the pipe and the process	*/
	FILE	*fdopen(), *fp;		/* fdopen makes a fd a stream	*/
	int	parent_end, child_end;	/* of pipe 			*/

	if ( *mode == 'r' ){		/* figure out direction		*/
		parent_end = READ;
		child_end = WRITE ;
	} else if ( *mode == 'w' ){
		parent_end = WRITE;
	    child_end = READ ;
	} else return NULL ;

	//todo	
}

int main()
{
	FILE	*fp;
	char	buf[BUFSIZ];

	fp = popen("ls","r");
	while( fgets(buf, BUFSIZ,fp) != NULL)
		fputs(buf, stdout);

	return 0;
}

