#include "inc/mred.h"
#include "inc/row_ops.h"
#include "inc/syntax_hl.h"

#include <string.h>

void
mred_insert_row(int at, char *s, size_t len)
{
	if (at < 0 || at > ED.numrows)
		return;

	edrow *newrow = realloc (ED.row, sizeof (edrow) * (ED.numrows + 1));
	if (newrow == NULL)
		die ("ERR: ED.row mem realloc");
	ED.row = newrow;

	memmove (&ED.row[at + 1], &ED.row[at],
			sizeof (edrow) * (ED.numrows - at));

	for (int j = at + 1; j <= ED.numrows; j++)
		ED.row[j].idx++;

	ED.row[at].idx = at;
	ED.row[at].size = len;

	ED.row[at].chars = malloc (len + 1);
	if (ED.row[at].chars == NULL)
		die ("ERR: ED.row chars mem alloc");
	memset (ED.row[at].chars, '\0', len + 1);

	memcpy (ED.row[at].chars, s, len);
	ED.row[at].chars[len] = '\0';

	ED.row[at].rsize = 0;
	ED.row[at].render = NULL;

	ED.row[at].hl = NULL;
	ED.row[at].hl_open_comment = 0;

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
	row->render = NULL;

	row->render = malloc (row->size + (tabs * (MRED_TAB_STOP - 1)) + 1);
	if (row->render == NULL)
		die ("ERR: row render malloc");

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
	mred_update_syntax (row);
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
	char *newchars = realloc (row->chars, row->size + 1);
	if (newchars == NULL)
		die ("ERR: insert row chars realloc");
	row->chars = newchars;
	memmove (&row->chars[at + 1], &row->chars[at], row->size - at + 1);
	row->size++;
	row->chars[at] = c;
	mred_update_row (row);
	ED.dirty = 1;
}


void
mred_row_del_char (edrow *row, int at)
{
	if (at < 0 || at >= row->size)
		return;
	memmove (&row->chars[at], &row->chars[at + 1], row->size - at);
	row->size--;
	mred_update_row (row);
	ED.dirty = 1;
}


void
mred_del_row (int at)
{
	if (at < 0 || at >= ED.numrows)
		return;
	mred_free_row (&ED.row[at]);
	memmove (&ED.row[at], &ED.row[at + 1],
			sizeof (edrow) * (ED.numrows - at - 1));
	for (int j = at; j < ED.numrows - 1; j++)
		ED.row[j].idx--;
	ED.numrows--;
	ED.dirty = 1;
}


void
mred_free_row (edrow *row)
{
	free (row->render);
	row->render = NULL;
	free (row->chars);
	row->chars = NULL;
	free (row->hl);
	row->hl = NULL;
}


void
mred_row_append_string (edrow *row, char *s, size_t len)
{
	char *newchars = realloc (row->chars, row->size + len + 1);
	if (newchars == NULL)
		die ("ERR: row append chars realloc");
	row->chars = newchars;
	memcpy (&row->chars[row->size], s, len);
	row->size += len;
	row->chars[row->size] = '\0';
	mred_update_row (row);
	ED.dirty = 1;
}


int
mred_row_rx_to_cx (edrow *row, int rx)
{
	int cur_rx = 0;
	int cx;
	for (cx = 0; cx < row->size; cx++)
	{
		if (row->chars[cx] == '\t')
			cur_rx += (MRED_TAB_STOP - 1) - (cur_rx % MRED_TAB_STOP);
		cur_rx++;
		if (cur_rx > rx)
			return cx;
	}
	return cx;
}
