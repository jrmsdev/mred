#include "inc/mred.h"
#include "inc/buffer.h"

void
ab_append (struct abuf *ab, const char *s, int len)
{
	char *new = realloc (ab->b, ab->len + len);
	if (new == NULL)
		return;
	memcpy (&new[ab->len], s, len);
	ab->b = new;
	ab->len += len;
}


void
ab_free (struct abuf *ab)
{
	free (ab->b);
	ab->b = NULL;
	ab->len = 0;
}
