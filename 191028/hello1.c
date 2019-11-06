/* hello1.c
 * purpose: show the minimal calls needed to use curses
 * outline: initialize, draw stuff, wait for input, quit
	*/

#include <stdio.h>
#include <curses.h>

int main()
{
	initscr();	// initialize curses library and tty
	clear();		// clear screen

	move(10, 20);
	addstr("Hello, World");	// draw string on the screen
	move(LINES - 1, 0);		// move to bottom left

	refresh();
	getch();

	endwin();	// turn off curses and reset the tty

	return 0;
}