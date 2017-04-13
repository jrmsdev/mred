#include "inc/mred.h"

void
mred_init ()
{
	enable_raw_mode ();
	ED.cx = 0;
	ED.cy = 0;
	ED.rx = 0;
	ED.numrows = 0;
	ED.row = NULL;
	ED.rowoff = 0;
	ED.coloff = 0;
	ED.filename = NULL;
	ED.statusmsg[0] = '\0';
	ED.statusmsg_time = 0;
	ED.dirty = 0;
	ED.syntax = NULL;
	ED.screenrows -= 2; /* make space for status and message bars */
	atexit (mred_free);
}


void
mred_free ()
{
	for (int i = 0; i < ED.numrows; i++)
	{
		if (ED.row[i].chars != NULL)
		{
			free (ED.row[i].chars);
			ED.row[i].chars = NULL;
		}
		if (ED.row[i].render != NULL)
		{
			free (ED.row[i].render);
			ED.row[i].render = NULL;
		}
		if (ED.row[i].hl != NULL)
		{
			free (ED.row[i].hl);
			ED.row[i].hl = NULL;
		}
	}
	if (ED.row != NULL)
	{
		free (ED.row);
		ED.row = NULL;
	}
	if (ED.filename != NULL)
	{
		free (ED.filename);
		ED.filename = NULL;
	}
}


void
die (const char *s)
{
	write (ED.stdout, "\x1b[2J", 4);
	write (ED.stdout, "\x1b[H", 3);
	perror (s);
	exit (1);
}
