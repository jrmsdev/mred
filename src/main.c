/*
** Created based on kilo tutorial:
**	http://viewsourcecode.org/snaptoken/kilo/
**	https://github.com/antirez/kilo
*/

#include "inc/mred.h"
#include "inc/file_io.h"
#include "inc/input.h"
#include "inc/output.h"

#include <string.h>

int
main (int argc, char *argv[]) {
	ED.stdout = STDOUT_FILENO;
	ED.stdin = STDIN_FILENO;

	if (argc == 2 && strncmp (argv[1], "--version", 9) == 0)
	{
		write (ED.stdout, "mred v", 6);
		write (ED.stdout, MRED_VERSION, strlen (MRED_VERSION));
		write (ED.stdout, "\r\n", 2);
		exit (0);
	}

	if (!MRED_ALLOW_NOTTY)
	{
		if (!isatty (ED.stdin) || !isatty(ED.stdout))
			die ("ERR: not running on a tty?");
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
