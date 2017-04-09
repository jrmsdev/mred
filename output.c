#include "mred.h"

void
mred_draw_rows (struct abuf *ab)
{
	int y;
	for (y = 0; y < ED.screenrows; y++)
	{
		int filerow = y + ED.rowoff;
		if (filerow >= ED.numrows)
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
			int len = ED.row[filerow].rsize - ED.coloff;
			if (len < 0)
				len = 0;
			if (len > ED.screencols)
				len = ED.screencols;
			ab_append (ab, &ED.row[filerow].render[ED.coloff], len);
		}
		ab_append (ab, "\x1b[K", 3); /* clear rest of the line */
		if (y < ED.screenrows -1)
			ab_append (ab, "\r\n", 2);
	}
}


void
mred_refresh_screen ()
{
	mred_scroll ();
	struct abuf ab = ABUF_INIT;
	ab_append (&ab, "\x1b[?25l", 6); /* hide cursor */
	ab_append (&ab, "\x1b[H", 3);
	mred_draw_rows (&ab);

	char buf[32];
	snprintf (buf, sizeof (buf), "\x1b[%d;%dH",
			(ED.cy - ED.rowoff) + 1, (ED.rx - ED.coloff) + 1);
	ab_append (&ab, buf, strlen (buf));

	ab_append (&ab, "\x1b[?25h", 6); /* show cursor */
	write (STDOUT_FILENO, ab.b, ab.len);
	ab_free (&ab);
}


void
mred_scroll ()
{
	ED.rx = 0;
	if (ED.cy < ED.numrows)
		ED.rx = mred_row_cx_to_rx (&ED.row[ED.cy], ED.cx);
	if (ED.cy < ED.rowoff)
		ED.rowoff = ED.cy;
	if (ED.cy >= ED.rowoff + ED.screenrows)
		ED.rowoff = ED.cy - ED.screenrows + 1;
	if (ED.rx < ED.coloff)
		ED.coloff = ED.rx;
	if (ED.rx >= ED.coloff + ED.screencols)
		ED.coloff = ED.rx - ED.screencols + 1;
}
