#include "mred.h"

void
mred_open(char *filename)
{
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
		mred_append_row (line, linelen);
	}
	free (line);
	fclose (fp);
}
