#include "mred.h"

static int
is_separator (int c)
{
	return isspace (c) || c == '\0' || strchr ("\",.()+-/*=~%<>[];", c) != NULL;
}

void
mred_update_syntax (edrow *row)
{
	row->hl = realloc (row->hl, row->rsize);
	memset (row->hl, HL_NORMAL, row->rsize);
	int prev_sep = 1;
	int i = 0;
	while (i < row->size)
	{
		char c = row->render[i];
		unsigned char prev_hl = (i > 0) ? row->hl[i - 1] : HL_NORMAL;
		if ((isdigit (c) && (prev_sep || prev_hl == HL_NUMBER)) ||
				(c == '.' && prev_hl == HL_NUMBER))
		{
			row->hl[i] = HL_NUMBER;
			i++;
			prev_sep = 0;
			continue;
		}
		prev_sep = is_separator (c);
		i++;
	}
}


int
mred_syntax_to_color (int hl)
{
	switch (hl)
	{
		case HL_NUMBER:
			return 31; /* red */
		case HL_MATCH:
			return 34; /* blue */
		default:
			return 37; /* white */
	}
}
