#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <termio.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

void set_cr_noecho_mode();
void echo_on();
void command_mode();
void insert_mode();
void tty_mode(int);
void rotate(int);

void main()
{
	// save current terminal mode
	tty_mode(0);
	
	// turn off canonical mode and echo mode of the terminal
	set_cr_noecho_mode();

	command_mode();

}


void set_cr_noecho_mode()
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

void echo_on()
{
	struct termios ttystate;

	tcgetattr(0, &ttystate);
	ttystate.c_lflag |= ECHO;
	tcsetattr(0, TCSANOW, &ttystate);	// install settings
	printf("\necho on!\n");
}

void echo_off()
{
	struct termios ttystate;

	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &ttystate);	// install settings
	printf("\necho off!\n");
}

void command_mode()
{
	int input;

	while (1) {
		input = getchar();

		if (input == 'i') {
			printf("\ninsert mode!\n");
			insert_mode();
		}
		else if (input == '1')
			echo_on();
		
		else if (input == '2')
			echo_off();
		
		else if (input == 'q') {
			// restore terminal mode
			tty_mode(1);
			// quit program
			exit(1);
		}
	}
}

void insert_mode()
{
	int input;

	while ((input = getchar()) != 27)	// while not ESC
		rotate(input);							// print

	// ESC
	printf("\ncommand mode!\n");
	command_mode();

}

void tty_mode(int how)
{
	static struct termios original_mode;
	static int original_flags;
	static int stored = 0;

	if (how == 0) {
		tcgetattr(0, &original_mode);
		original_flags = fcntl(0, F_GETFL);
		stored = 1;
	}
	else if (stored) {
		tcsetattr(0, TCSANOW, &original_mode);
		original_flags = fcntl(0, F_SETFL, original_flags);
	}
}



void rotate(int c)
{
	if (c == 'z')
		c = 'a';
	else if (islower(c))			
		c++;

	putchar(c);
}