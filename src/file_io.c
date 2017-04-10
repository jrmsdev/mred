#include "mred.h"

void
mred_open(char *filename)
{
	free (ED.filename);
	ED.filename = strdup (filename);
	mred_select_syntax_hl ();
	FILE *fp = fopen (filename, "r");
	if (!fp)
		die ("ERR: fopen");
	char *line = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	while ((linelen = getline (&line, &linecap, fp)) != -1)
	{
		if (linelen > 0 && (line[linelen - 1] == '\n' ||
				line[linelen - 1] == '\r'))
			linelen--;
		mred_insert_row (ED.numrows, line, linelen);
	}
	free (line);
	fclose (fp);
	ED.dirty = 0;
}


char *
mred_rows_to_string (int *buflen)
{
	int totlen = 0;
	int j;
	for (j = 0; j < ED.numrows; j++)
		totlen += ED.row[j].size + 1;
	*buflen = totlen;

	char *buf = malloc (totlen);
	char *p = buf;
	for (j = 0; j < ED.numrows; j++)
	{
		memcpy (p, ED.row[j].chars, ED.row[j].size);
		p += ED.row[j].size;
		*p = '\n';
		p++;
	}
	return buf;
}


void
mred_save ()
{
	if (ED.filename == NULL)
	{
		ED.filename = mred_prompt ("Save as: %s (ESC to cancel)", NULL);
		if (ED.filename == NULL)
		{
			mred_set_status_message ("Save aborted");
			return;
		}
		mred_select_syntax_hl ();
	}
	int len;
	char *buf = mred_rows_to_string (&len);
	int fd = open (ED.filename, O_RDWR | O_CREAT, 0644);
	if (fd != -1)
	{
		if (ftruncate (fd, len) != -1)
		{
			if (write (fd, buf, len) == len)
			{
				close (fd);
				free (buf);
				ED.dirty = 0;
				mred_set_status_message ("%d bytes saved", len);
				return;
			}
		}
		close (fd);
	}
	free (buf);
	mred_set_status_message ("Save failed! I/O error: %s",
			strerror (errno));
}
