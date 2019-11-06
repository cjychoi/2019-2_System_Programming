/*
 * purpose: show how to use curses functions with a loop
 * outline: initialize, draw stuff, wrap up
 */

#include <stdio.h>
#include <curses.h>
#include <unistd.h>

int main()
{
	int i;

	initscr();	// turn on curses
	clear();

	for (i = 0; i < LINES; i++) {
		move(i, i + i);
		if (i % 2 == 1)
			standout();
		addstr("Hello, world");
		if (i % 2 == 1)
			standend();
	}

	refresh();	// update the screen
	sleep(3);
	endwin();

	return 0;
}