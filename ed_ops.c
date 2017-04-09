#include "mred.h"

void
mred_insert_char (int c)
{
	if (ED.cy == ED.numrows)
		mred_append_row ("", 0);
	mred_row_insert_char (&ED.row[ED.cy], ED.cx, c);
	ED.cx++;
}
