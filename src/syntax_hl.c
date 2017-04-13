#include "inc/mred.h"
#include "inc/filetypes.h"
#include "inc/syntax_hl.h"

#include <ctype.h>
#include <string.h>

static int
is_separator (int c)
{
	return isspace (c) || c == '\0' || strchr (",.()+-/*=~%<>[];:", c) != NULL;
}


void
mred_update_syntax (edrow *row)
{
	unsigned char *newhl = realloc (row->hl, row->rsize);
	if (newhl == NULL)
		die ("ERR: update syntax new hl realloc");
	row->hl = newhl;
	memset (row->hl, HL_NORMAL, row->rsize);
	if (ED.syntax == NULL)
		return;
	char **keywords = ED.syntax->keywords;
	char *scs = ED.syntax->singleline_comment_start;
	char *mcs = ED.syntax->multiline_comment_start;
	char *mce = ED.syntax->multiline_comment_end;
	int scs_len = scs ? strlen (scs) : 0;
	int mcs_len = mcs ? strlen (mcs) : 0;
	int mce_len = mce ? strlen (mce) : 0;
	int prev_sep = 1;
	int in_string = 0;
	int in_comment = (row->idx > 0 && ED.row[row->idx - 1].hl_open_comment);
	int i = 0;
	while (i < row->rsize)
	{
		char c = row->render[i];
		unsigned char prev_hl = (i > 0) ? row->hl[i - 1] : HL_NORMAL;

		if (scs_len && !in_string && !in_comment)
		{
			if (!strncmp (&row->render[i], scs, scs_len))
			{
			memset (&row->hl[i], HL_COMMENT,
						row->rsize - i);
				break;
			}
		}

		if (mcs_len && mce_len && !in_string)
		{
			if (in_comment)
			{
				row->hl[i] = HL_MLCOMMENT;
				if (!strncmp (&row->render[i], mce, mce_len))
				{
					memset (&row->hl[i], HL_MLCOMMENT, mce_len);
					i += mce_len;
					in_comment = 0;
					prev_sep = 1;
					continue;
				}
				else
				{
					i++;
					continue;
				}
			}
			else if (!strncmp (&row->render[i], mcs, mcs_len))
			{
				memset (&row->hl[i], HL_MLCOMMENT, mcs_len);
				i += mcs_len;
				in_comment = 1;
				continue;
			}
		}

		if (ED.syntax->flags & HL_DO_STRINGS)
		{
			if (in_string)
			{
				row->hl[i] = HL_STRING;
				if (c == '\\' && i + 1 < row->rsize)
				{
					row->hl[i + 1] = HL_STRING;
					i += 2;
					continue;
				}
				if (c == in_string)
					in_string = 0;
				i++;
				prev_sep = 1;
				continue;
			}
			else
			{
				if (c == '"' || c == '\'')
				{
					in_string = c;
					row->hl[i] = HL_STRING;
					i++;
					continue;
				}
			}
		}

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

		if (prev_sep)
		{
			int j;
			for (j = 0; keywords[j]; j++)
			{
				int klen = strlen (keywords[j]);
				int kw2 = keywords[j][klen - 1] == '|';
				if (kw2)
					klen--;
				if (!strncmp (&row->render[i], keywords[j], klen) &&
						is_separator (row->render[i + klen]))
				{
					memset (&row->hl[i], kw2 ? HL_KEYWORD2 : HL_KEYWORD1, klen);
					i += klen;
					break;
				}
			}
			if (keywords[j] != NULL)
			{
				prev_sep = 0;
				continue;
			}
		}

		prev_sep = is_separator (c);
		i++;
	}

	int changed = (row->hl_open_comment != in_comment);
	row->hl_open_comment = in_comment;
	if (changed && row->idx + 1 < ED.numrows)
		mred_update_syntax (&ED.row[row->idx + 1]);
}


int
mred_syntax_to_color (int hl)
{
	switch (hl)
	{
		case HL_COMMENT:
		case HL_MLCOMMENT:
			return 36; /* cyan */
		case HL_KEYWORD1:
			return 33; /* yellow */
		case HL_KEYWORD2:
			return 32; /* green */
		case HL_STRING:
			return 35; /* magenta */
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
