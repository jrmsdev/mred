#include "inc/mred.h"
#include "inc/ed_ops.h"
#include "inc/row_ops.h"

void
mred_insert_char (int c)
{
	if (ED.cy == ED.numrows)
		mred_insert_row (ED.cy, "", 0);
	mred_row_insert_char (&ED.row[ED.cy], ED.cx, c);
	ED.cx++;
}


void
mred_del_char ()
{
	if (ED.cy == ED.numrows)
		return;
	if (ED.cx == 0 && ED.cy == 0)
		return;
	edrow *row = &ED.row[ED.cy];
	if (ED.cx > 0)
	{
		mred_row_del_char (row, ED.cx - 1);
		ED.cx--;
	}
	else
	{
		ED.cx = ED.row[ED.cy - 1].size;
		mred_row_append_string (&ED.row[ED.cy - 1],
				row->chars, row->size);
		mred_del_row (ED.cy);
		ED.cy--;
	}
}


void
mred_insert_newline ()
{
	if (ED.cx == 0)
	{
		mred_insert_row (ED.cy, "", 0);
	}
	else
	{
		edrow *row = &ED.row[ED.cy];
		mred_insert_row (ED.cy + 1,
				&row->chars[ED.cx], row->size - ED.cx);
		row = &ED.row[ED.cy];
		row->size = ED.cx;
		row->chars[row->size] = '\0';
		mred_update_row (row);
	}
	ED.cy++;
	ED.cx = 0;
}
