#include <stdio.h>
#include <termio.h>

#define QUESTION "Do you want another transaction"
int get_response(char *);
void set_cr_noecho_mode(void);
void tty_mode(int);

int main()
{
	int response;
	tty_mode(0);											// save tty mode (Terminal driver mode)
	set_cr_noecho_mode();							// set -canon, -echo
	response = get_response(QUESTION);	// get answer
	tty_mode(1);											// restore tty mode
	return response;
}

int get_response(char *question)
/* purpose: ask a question and wait for a y/n answer
*   method: use getchar and ignore non y/n answers
*	 returns: 0-> yes, 1->no
*/
{
	int input;
	printf("%s (y/n)?", question);
	while (1) {
		switch (input = getchar()) {
		case 'y':
		case 'Y': return 0;
		case 'n':
		case 'N':
		case EOF: return 1;
		}
	}
}

void set_crmode(void)
/*
 * purpose: put file descriptor 0(i.e. stdin) into chr-by-chr mode
 * method: use bits in termios
 */
{
	struct termios ttystate;

	tcgetattr(0, &ttystate);				// read curr. setting
	ttystate.c_lflag &= ~ICANON;	// no buffering
	ttystate.c_cc[VMIN] = 1;			// get 1 char at a time

	tcsetattr(0, TCSANOW, &ttystate);	// install settings
}

/* how == 0 -> save current mode, how ==1 -> restore mode */
void tty_mode(int how)
{
	static struct termios original_mode;
	if (how == 0)
		tcgetattr(0, &original_mode);
	else
		tcsetattr(0, TCSANOW, &original_mode);
}

void set_cr_noecho_mode(void)
/*
 * purpose: put file descriptor 0 into chr-by-chr mode and noecho mode
 * method: use bits in termios
*/
{
	struct termios ttystate;

	tcgetattr(0, &ttystate);						// read curr. setting
	ttystate.c_lflag &= ~ICANON;			// no buffering
	ttystate.c_lflag &= ~ECHO;				// no echo
	ttystate.c_cc[VMIN] = 1;					// get 1 char at a time
	tcsetattr(0, TCSANOW, &ttystate);	// install settings
}
