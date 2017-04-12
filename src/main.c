/* Created based on:
**   http://viewsourcecode.org/snaptoken/kilo/
**   https://github.com/antirez/kilo
*/

#include "mred.h"

int
main (int argc, char *argv[]) {
	if (argc == 2 && strncmp (argv[1], "--version", 9) == 0)
	{
		write (STDOUT_FILENO, "mred v", 6);
		write (STDOUT_FILENO, MRED_VERSION, strlen (MRED_VERSION));
		write (STDOUT_FILENO, "\r\n", 2);
		exit (0);
	}
	mred_init ();
	if (argc == 2)
		mred_open (argv[1]);

	mred_set_status_message (
			"HELP: Ctrl-Q = quit | Ctrl-S = save | Ctrl-F = find");

	while (1)
	{
		mred_refresh_screen ();
		mred_process_keypress ();
	}
	return (0);
}
