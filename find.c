#include "mred.h"

void
mred_find_callback (char *query, int key)
{
	static int last_match = -1;
	static int direction = 1;
	if (key == '\r' || key == '\x1b')
	{
		last_match = -1;
		direction = 1;
		return;
	}
	else if (key == ARROW_RIGHT || key == ARROW_DOWN)
	{
		direction = 1;
	}
	else if (key == ARROW_LEFT || key == ARROW_UP)
	{
		direction = -1;
	}
	else
	{
		last_match = -1;
		direction = 1;
	}
	int current = last_match;
	int i;
	for (i = 0; i < ED.numrows; i++)
	{
		current += direction;
		if (current == -1)
			current = ED.numrows -1;
		else if (current == ED.numrows)
			current = 0;
		edrow *row = &ED.row[current];
		char *match = strstr (row->render, query);
		if (match)
		{
			last_match = current;
			ED.cy = current;
			ED.cx = mred_row_rx_to_cx (row, match - row->render);
			ED.rowoff = ED.numrows;
			break;
		}
	}
}


void
mred_find ()
{
	int saved_cx = ED.cx;
	int saved_cy = ED.cy;
	int saved_coloff = ED.coloff;
	int saved_rowoff = ED.rowoff;
	char *query = mred_prompt ("Search: %s (Use ESC/Arrows/Enter)",
			mred_find_callback);
	if (query)
	{
		free (query);
	}
	else
	{
		ED.cx = saved_cx;
		ED.cy = saved_cy;
		ED.coloff = saved_coloff;
		ED.rowoff = saved_rowoff;
	}
}
