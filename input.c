#include "mred.h"

void mred_process_keypress ()
{
	int c = mred_read_key ();
	switch (c)
	{
		case CTRL_KEY ('q'):
			write (STDOUT_FILENO, "\x1b[2J", 4);
			write (STDOUT_FILENO, "\x1b[H", 3);
			exit (0);
			break;
		case HOME_KEY:
			ED.cx = 0;
			break;
		case END_KEY:
			ED.cx = ED.screencols - 1;
			break;
		case PAGE_UP:
		case PAGE_DOWN:
			{
				int times = ED.screenrows;
				while (times--)
					mred_move_cursor (c == PAGE_UP ?
							ARROW_UP : ARROW_DOWN);
			}
		case ARROW_UP:
		case ARROW_DOWN:
		case ARROW_LEFT:
		case ARROW_RIGHT:
			mred_move_cursor (c);
			break;
	}
}


void
mred_move_cursor (int key)
{
	switch (key)
	{
		case ARROW_LEFT:
			if (ED.cx != 0)
				ED.cx--;
			break;
		case ARROW_RIGHT:
			if (ED.cx != ED.screencols - 1)
				ED.cx++;
			break;
		case ARROW_UP:
			if (ED.cy != 0)
				ED.cy--;
			break;
		case ARROW_DOWN:
			if (ED.cy != ED.screenrows - 1)
				ED.cy++;
			break;
	}
}
