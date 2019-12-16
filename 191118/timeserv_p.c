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
	struct sockaddr_in saddr;	/* build our address here	*/	// IP Port Number ����
	struct hostent *hp;		/* this part of our		*/
	char hostname[HOSTLEN];		/* address			*/
	int sock_id, sock_fd;		/* line id, file descriptor	*/
	FILE *sock_fp;			/* use socket as steam		*/
	char *ctime();			/* convert secs to string	*/
	time_t thetime;			/* the time we report		*/
    
	/* Step 1: ask kernel for a socket */
	sock_id = socket(PF_INET, SOCK_STREAM, 0);	// ���� ����
	if (sock_id == -1)
		oops("socket");

	/* Step 2: bind address to socket. Address is host, port */
	bzero ( (void*) &saddr, sizeof(saddr) );	// clear out struct - ���� 0���� �ʱ�ȭ
	
	gethostname(hostname, HOSTLEN);	// where am I?	- ���� ��ǻ���� hostname
	hp = gethostbyname(hostname);			// get info about host - hostname�� ���� host�� ���� ����(address ��) �о����

	bcopy( (void*) hp->h_addr, (void*) &saddr.sin_addr, hp->h_length);	// h_addr: IP �ּ� -> saddr.sin_addr�� ����
	saddr.sin_port = htons(PORTNUM);		// fill in socket port
	saddr.sin_family = AF_INET;					// fill in addr family - IP�ּ��� ���� ����

	if(bind(sock_id, (struct sockaddr *) &saddr, sizeof(saddr)) != 0)	// �ּ� �Ҵ�
		oops("bind");


	/* Step 3: allow incoming calls with Qsize=1 on socket */
	if (listen(sock_id, 1) != 0)
		oops("listen");

	/* main loop: accept(), write(), close() */

	while (1) {	// �� client ���� accept() ���� ���� �ݺ�
		sock_fd = accept(sock_id, NULL, NULL);		// wait for call
		printf("Wow! got a call!\n");
		if (sock_fd == -1)
			oops("accept");		// error getting calls

		sock_fp = fdopen(sock_fd, "w");	// we'll write to the - filestream�� �����Ͽ� file pointer ��ȯ - �������� ��� ���, ����
		if (sock_fp == NULL)		// socket as a stream
			oops("fdopen");			// unless we can't

		thetime = time(NULL);		// get time
		// and convert to string
		fprintf(sock_fp, "The time here is ..");		// client���� ����
		fprintf(sock_fp, "%s", ctime(&thetime));
		fclose(sock_fp);		// release connection
	}

	
}
