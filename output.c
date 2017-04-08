#include "mred.h"

void
mred_draw_rows (struct abuf *ab)
{
	int y;
	for (y = 0; y < ED.screenrows; y++)
	{
		if (y >= ED.numrows)
		{
			if (ED.numrows == 0 && y == ED.screenrows / 3)
			{
				char welcome[80];
				int welcomelen = snprintf (welcome,
						sizeof (welcome),
						"Mr. ED v%s", MRED_VERSION);
				if (welcomelen > ED.screencols)
					welcomelen = ED.screencols;
				int padding = (ED.screencols - welcomelen) / 2;
				if (padding)
				{
					ab_append (ab, "~", 1);
					padding--;
				}
				while (padding--)
					ab_append (ab, " ", 1);
				ab_append (ab, welcome, welcomelen);
			}
			else
			{
				ab_append (ab, "~", 1);
			}
		}
		else
		{
			int len = ED.row[y].size;
			if (len > ED.screencols)
				len = ED.screencols;
			ab_append (ab, ED.row[y].chars, len);
		}
		ab_append (ab, "\x1b[K", 3); /* clear rest of the line */
		if (y < ED.screenrows -1)
			ab_append (ab, "\r\n", 2);
	}
}


void
mred_refresh_screen ()
{
	struct abuf ab = ABUF_INIT;
	ab_append (&ab, "\x1b[?25l", 6); /* hide cursor */
	ab_append (&ab, "\x1b[H", 3);
	mred_draw_rows (&ab);

	char buf[32];
	snprintf (buf, sizeof (buf), "\x1b[%d;%dH", ED.cy + 1, ED.cx + 1);
	ab_append (&ab, buf, strlen (buf));

	ab_append (&ab, "\x1b[?25h", 6); /* show cursor */
	write (STDOUT_FILENO, ab.b, ab.len);
	ab_free (&ab);
}
