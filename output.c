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
			char *c = &ED.row[filerow].render[ED.coloff];
			unsigned char *hl = &ED.row[filerow].hl[ED.coloff];
			int cur_color = -1;
			for (int j = 0; j < len; j++)
			{
				if (hl[j] == HL_NORMAL)
				{
					if (cur_color != -1)
					{
						ab_append (ab, "\x1b[39m", 5);
						cur_color = -1;
					}
					ab_append (ab, &c[j], 1);
				}
				else
				{
					int color = mred_syntax_to_color (hl[j]);
					if (color != cur_color)
					{
						cur_color = color;
						char buf[16];
						int clen = snprintf (buf, sizeof (buf),
								"\x1b[%dm", color);
						ab_append (ab, buf, clen);
					}
					ab_append (ab, &c[j], 1);
				}
			}
			ab_append (ab, "\x1b[39m", 5);
		}
		ab_append (ab, "\x1b[K", 3); /* clear rest of the line */
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
	mred_draw_status_bar (&ab);
	mred_draw_message_bar (&ab);

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


void
mred_draw_status_bar (struct abuf *ab)
{
	ab_append (ab, "\x1b[7m", 4); /* inverted colors */
	char status[80];
	char rstatus[80];
	int len = snprintf (status, sizeof (status), "%.20s - %d lines %s",
			ED.filename ? ED.filename : "[No Name]", ED.numrows,
			ED.dirty ? "(modified)" : "");
	int rlen = snprintf (rstatus, sizeof (rstatus), "%s | %d/%d",
			ED.syntax ? ED.syntax->filetype : "no ft",
			ED.cy + 1, ED.numrows);
	if (len > ED.screencols)
		len = ED.screencols;
	ab_append (ab, status, len);
	while (len < ED.screencols)
	{
		if (ED.screencols - len == rlen)
		{
			ab_append (ab, rstatus, rlen);
			break;
		}
		else
		{
			ab_append (ab, " ", 1);
			len++;
		}
	}
	ab_append (ab, "\x1b[m", 3); /* get back to normal colors */
	ab_append (ab, "\r\n", 2);
}


void
mred_set_status_message (const char *fmt, ...)
{
	va_list ap;
	va_start (ap, fmt);
	vsnprintf (ED.statusmsg, sizeof (ED.statusmsg), fmt, ap);
	va_end (ap);
	ED.statusmsg_time = time(NULL);
}


void
mred_draw_message_bar (struct abuf *ab)
{
	ab_append (ab, "\x1b[K", 3);
	int msglen = strlen (ED.statusmsg);
	if (msglen > ED.screencols)
		msglen = ED.screencols;
	if (msglen && time(NULL) - ED.statusmsg_time < 5)
		ab_append (ab, ED.statusmsg, msglen);
}
