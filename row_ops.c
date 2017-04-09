#include "mred.h"

void
mred_append_row(char *s, size_t len)
{
	ED.row = realloc (ED.row, sizeof (edrow) * (ED.numrows + 1));
	int at = ED.numrows;
	ED.row[at].size = len;
	ED.row[at].chars = malloc (len + 1);
	memcpy (ED.row[at].chars, s, len);
	ED.row[at].chars[len] = '\0';
	ED.row[at].rsize = 0;
	ED.row[at].render = NULL;
	mred_update_row (&ED.row[at]);
	ED.numrows++;
	ED.dirty = 1;
}


void
mred_update_row (edrow *row)
{
	int tabs = 0;
	int j;
	for (j = 0; j < row->size; j++)
		if (row->chars[j] == '\t')
			tabs++;
	free (row->render);
	row->render = malloc (row->size + (tabs * (MRED_TAB_STOP - 1)) + 1);
	int idx = 0;
	for (j = 0; j < row->size; j++)
	{
		if (row->chars[j] == '\t')
		{
			row->render[idx++] = ' ';
			while (idx % MRED_TAB_STOP != 0)
				row->render[idx++] = ' ';
		}
		else
		{
			row->render[idx++] = row->chars[j];
		}
	}
	row->render[idx] = '\0';
	row->rsize = idx;
}


int
mred_row_cx_to_rx (edrow *row, int cx)
{
	int rx = 0;
	int j;
	for (j = 0; j < cx; j++)
	{
		if (row->chars[j] == '\t')
			rx += (MRED_TAB_STOP - 1) - (rx % MRED_TAB_STOP);
		rx++;
	}
	return rx;
}


void
mred_row_insert_char (edrow *row, int at, int c)
{
	if (at < 0 || at > row->size)
		at = row->size;
	row->chars = realloc (row->chars, row->size + 1);
	memmove (&row->chars[at + 1], &row->chars[at], row->size - at + 1);
	row->size++;
	row->chars[at] = c;
	mred_update_row (row);
	ED.dirty = 1;
}
