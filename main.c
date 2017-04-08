/* Created based on:
**   http://viewsourcecode.org/snaptoken/kilo/
**   https://github.com/antirez/kilo
**/

#include "mred.h"

int
main () {
	enable_raw_mode ();
	mred_init ();
	while (1)
	{
		mred_refresh_screen ();
		mred_process_keypress ();
	}
	return (0);
}


void
die (const char *s)
{
	write (STDOUT_FILENO, "\x1b[2J", 4);
	write (STDOUT_FILENO, "\x1b[H", 3);
	perror (s);
	exit (1);
}


void
mred_init ()
{
	ED.cx = 0;
	ED.cy = 0;
	if (get_window_size (&ED.screenrows, &ED.screencols) == -1)
		die ("ERR: get window size");
}
