#include "inc/mred.h"

void
mred_find_callback (char *query, int key)
{
	static int last_match = -1;
	static int direction = 1;
	static int saved_hl_line;
	static char *saved_hl = NULL;
	if (saved_hl)
	{
		memcpy (ED.row[saved_hl_line].hl, saved_hl,
				ED.row[saved_hl_line].rsize);
		free (saved_hl);
		saved_hl = NULL;
	}
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
			saved_hl_line = current;

			saved_hl = malloc (row->rsize);
			if (saved_hl == NULL)
				die ("ERR: highlight save malloc");

			memcpy (saved_hl, row->hl, row->rsize);
			memset (&row->hl[match - row->render],
					HL_MATCH, strlen(query));
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
		query = NULL;
	}
	else
	{
		ED.cx = saved_cx;
		ED.cy = saved_cy;
		ED.coloff = saved_coloff;
		ED.rowoff = saved_rowoff;
	}
}
