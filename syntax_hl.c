#include "mred.h"
#include "filetypes.h"

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
	if (ED.syntax == NULL)
		return;
	int prev_sep = 1;
	int i = 0;
	while (i < row->size)
	{
		char c = row->render[i];
		unsigned char prev_hl = (i > 0) ? row->hl[i - 1] : HL_NORMAL;
		if (ED.syntax->flags & HL_DO_NUMBERS)
		{
			if ((isdigit (c) && (prev_sep || prev_hl == HL_NUMBER))
					|| (c == '.' && prev_hl == HL_NUMBER))
			{
				row->hl[i] = HL_NUMBER;
				i++;
				prev_sep = 0;
				continue;
			}
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


void
mred_select_syntax_hl ()
{
	ED.syntax = NULL;
	if (ED.filename == NULL)
		return;
	for (unsigned int j = 0; j < HLDB_ENTRIES; j++)
	{
		struct mred_syntax *s = &HLDB[j];
		unsigned int i = 0;
		while (s->filematch[i])
		{
			char *p = strstr (ED.filename, s->filematch[i]);
			if (p != NULL)
			{
				int patlen = strlen (s->filematch[i]);
				if (s->filematch[i][0] != '.' || p[patlen] == '\0')
				{
					ED.syntax = s;
					int frow;
					for (frow = 0; frow < ED.numrows; frow++)
					{
						mred_update_syntax (&ED.row[frow]);
					}
					return;
				}
			}
			i++;
		}
	}
}
