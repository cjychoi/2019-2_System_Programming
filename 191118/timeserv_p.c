/* timeserv.c - a socket-based time of day server
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define PORTNUM 13000 
#define HOSTLEN 256
#define oops(msg)	{ perror(msg); exit(1); }

int main(int ac, char *av[])
{
	struct sockaddr_in saddr;	/* build our address here	*/
	struct hostent *hp;		/* this part of our		*/
	char hostname[HOSTLEN];		/* address			*/
	int sock_id, sock_fd;		/* line id, file descriptor	*/
	FILE *sock_fp;			/* use socket as steam		*/
	char *ctime();			/* convert secs to string	*/
	time_t thetime;			/* the time we report		*/
    //todo
	
}
