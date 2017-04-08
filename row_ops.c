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
	ED.numrows++;
}
