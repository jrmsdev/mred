#include "inc/mred.h"

void mred_process_keypress ()
{
	static int quit_times = MRED_QUIT_TIMES;
	int c = mred_read_key ();
	switch (c)
	{
		case '\r':
			mred_insert_newline ();
			break;
		case CTRL_KEY ('q'):
			if (ED.dirty && quit_times > 0)
			{
				mred_set_status_message("WARNING! Unsaved changes. Press Ctrl-Q %d more times to quit.", quit_times);
				quit_times--;
				return;
			}
			write (ED.stdout, "\x1b[2J", 4);
			write (ED.stdout, "\x1b[H", 3);
			exit (0);
			break;
		case CTRL_KEY ('s'):
			mred_save ();
			break;
		case HOME_KEY:
			ED.cx = 0;
			break;
		case END_KEY:
			if (ED.cy < ED.numrows)
				ED.cx = ED.row[ED.cy].size;
			break;
		case CTRL_KEY ('f'):
			mred_find ();
			break;
		case BACKSPACE:
		case CTRL_KEY ('h'):
		case DEL_KEY:
			if (c == DEL_KEY)
				mred_move_cursor (ARROW_RIGHT);
			mred_del_char ();
			break;
		case PAGE_UP:
		case PAGE_DOWN:
			{
				if (c == PAGE_UP)
					ED.cy = ED.rowoff;
				else if (c == PAGE_DOWN)
					ED.cy = ED.rowoff + ED.screenrows - 1;
				int times = ED.screenrows;
				while (times--)
					mred_move_cursor (c == PAGE_UP ?
							ARROW_UP : ARROW_DOWN);
			}
			break;
		case ARROW_UP:
		case ARROW_DOWN:
		case ARROW_LEFT:
		case ARROW_RIGHT:
			mred_move_cursor (c);
			break;
		case CTRL_KEY ('l'):
		case '\x1b':
			break;
		default:
			mred_insert_char (c);
			break;
	}
	quit_times = MRED_QUIT_TIMES;
}


void
mred_move_cursor (int key)
{
	edrow *row = (ED.cy >= ED.numrows) ? NULL : &ED.row[ED.cy];
	switch (key)
	{
		case ARROW_LEFT:
			if (ED.cx != 0)
			{
				ED.cx--;
			}
			else if (ED.cy > 0)
			{
				ED.cy--;
				ED.cx = ED.row[ED.cy].size;
			}
			break;
		case ARROW_RIGHT:
			if (row && ED.cx < row->size)
			{
				ED.cx++;
			}
			else if (row && ED.cx == row->size)
			{
				ED.cy++;
				ED.cx = 0;
			}
			break;
		case ARROW_UP:
			if (ED.cy != 0)
				ED.cy--;
			break;
		case ARROW_DOWN:
			if (ED.cy < ED.numrows)
				ED.cy++;
			break;
	}
	row = (ED.cy >= ED.numrows) ? NULL : &ED.row[ED.cy];
	int rowlen = row ? row->size : 0;
	if (ED.cx > rowlen)
		ED.cx = rowlen;
}


char *
mred_prompt (char *prompt, void (*callback)(char *, int))
{
	size_t buflen = 0;
	size_t bufsize = 128;
	char *buf = malloc (bufsize);

	if (buf == NULL)
		die ("ERR: prompt buf malloc");

	buf[0] = '\0';
	while (1)
	{
		mred_set_status_message (prompt, buf);
		mred_refresh_screen ();
		int c = mred_read_key ();
		if (c == DEL_KEY || c == CTRL_KEY ('h') || c == BACKSPACE)
		{
			if (buflen != 0)
				buf[--buflen] = '\0';
		}
		else if (c == '\x1b')
		{
			mred_set_status_message ("");
			if (callback)
				callback (buf, c);
			free (buf);
			buf = NULL;
			return NULL;
		}
		else if (c == '\r')
		{
			if (buflen != 0)
			{
				mred_set_status_message ("");
				if (callback)
					callback (buf, c);
				return buf;
			}
		}
		else if (!iscntrl (c) && c < 128)
		{
			if (buflen == bufsize -1)
			{
				bufsize *= 2;
				char *newbuf = realloc (buf, bufsize);
				if (newbuf == NULL)
					die ("ERR: prompt buf realloc");
				buf = newbuf;
			}
			buf[buflen++] = c;
			buf[buflen] = '\0';
		}
		if (callback)
			callback (buf, c);
	}
}
